// SPDX-License-Identifier: GPL-2.0
/*
 * Clang Control Flow Integrity (CFI) support.
 *
 * Copyright (C) 2023 Google LLC
 */
#include <asm/cfi.h>
#include <asm/insn.h>

/*
 * Returns the targew Integr.h>
#include <linux/cpu.h>
#include <linux/uaccess.h>
#include <asm/alternative.h>
#include <asm/module.h>
#include <asm/sections.h>
#include <asm/vdso.h>
#include <asm/vendorid_list.h>
#include <asm/sbi.h>
#include <asm/csr.h>
#include <asm/insn.h>
#include <asm/patch.h>

struct cpu_manufacturer_info_t {
	unsigned long vendor_id;
	unsigned long arch_id;
	unsigned long imp_id;
	void (*patch_func)(struct alt_entry *begin, struct alt_entry *end,
				  unsigned long archid, unsigned long impid,
				  unsigned int stage);
};

static void riscv_fill_cpu_mfr_info(struct cpu_manufacturer_info_t *cpu_mfr_info)
{
#ifdef CONFIG_RISCV_M_MODE
	cpu_mfr_info->vendor_id = csr_read(CSR_MVENDORID);
	cpu_mfr_info->arch_id = csr_read(CSR_MARCHID);
	cpu_mfr_info->imp_id = csr_read(CSR_MIMPID);
#else
	cpu_mfr_info->vendor_id = sbi_get_mvendorid();
	cpu_mfr_info->arch_id = sbi_get_marchid();
	cpu_mfr_info->imp_id = sbi_get_mimpid();
#endif

	switch (cpu_mfr_info->vendor_id) {
#ifdef CONFIG_ERRATA_ANDES
	case ANDESTECH_VENDOR_ID:
		cpu_mfr_info->patch_func = andes_errata_patch_func;
		break;
#endif
#ifdef CONFIG_ERRATA_SIFIVE
	case SIFIVE_VENDOR_ID:
		cpu_mfr_info->patch_func = sifive_errata_patch_func;
		break;
#endif
#ifdef CONFIG_ERRATA_THEAD
	case THEAD_VENDOR_ID:
		cpu_mfr_info->patch_func = thead_errata_patch_func;
		break;
#endif
	default:
		cpu_mfr_info->patch_func = NULL;
	}
}

static u32 riscv_instruction_at(void *p)
{
	u16 *parcel = p;

	return (u32)parcel[0] | (u32)parcel[1] << 16;
}

static void riscv_alternative_fix_auipc_jalr(void *ptr, u32 auipc_insn,
					     u32 jalr_insn, int patch_offset)
{
	u32 call[2] = { auipc_insn, jalr_insn };
	s32 imm;

	/* get and adju