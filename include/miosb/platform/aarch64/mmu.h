#include "miosb/bits.h"

/* D10.2.100 SCTLR_EL1, System Control Register (EL1) */
#define SCTLR_EL1_M BIT(0)


/* D10.2.103 TCR_EL1, Translation Control Register (EL1) */
#define TCR_EL1_IPS_MASK GENMASK(34, 32)
#define TCR_EL1_TG1_MASK GENMASK(31, 30)
#define TCR_EL1_SH1_MASK GENMASK(29, 28)
#define TCR_EL1_ORGN1_MASK GENMASK(27, 26)
#define TCR_EL1_IRGN1_MASK GENMASK(25, 24)
#define TCR_EL1_EPD1 BIT(23)
#define TCR_EL1_A1 BIT(22)
#define TCR_EL1_T1SZ_MASK GENMASK(21, 16)
#define TCR_EL1_TG0_MASK GENMASK(15, 14)
#define TCR_EL1_SH0_MASK GENMASK(13, 12)
#define TCR_EL1_ORGN0_MASK GENMASK(11, 10)
#define TCR_EL1_IRGN0_MASK GENMASK(9, 8)
#define TCR_EL1_EPD0 BIT(7)
#define TCR_EL1_T0SZ_MASK GENMASK(5, 0)

#define TCR_EL1_TG0_4K 0b00
#define TCR_EL1_SH0_INNER 0b11
#define TCR_EL1_RGN_WBWA 0b01 /* Normal memory, Inner Write-Back Read-Allocate Write-Allocate Cacheable */

/* D10.2.82 MAIR_EL1, Memory Attribute Indirection Register (EL1) */
#define MAIR_EL1_ATTR_DEVICE_nGnRnE UL(0x00) /* Device-nGnRnE */
#define MAIR_EL1_ATTR_NORMAL_NC     UL(0x44) /* Normal, Inner/Outer Non-cacheable */
#define MAIR_EL1_ATTR_NORMAL        UL(0xff) /* Normal, Inner/Outer Write-Back Non-transient, R/W-Allocate */
#define MAIR_EL1_ATTR_MASK          UL(0xff)

/* Place attr at Attr<idx> */
#define MAIR_EL1_ATTRIDX(attr, idx) ((attr) << ((idx) * 8))

/* D10.2.54 ID_AA64MMFR0_EL1, AArch64 Memory Model Feature Register 0 */
#define ID_AA64MMFR0_EL1_PARANGE_MASK GENMASK(3, 0)

#define ID_AA64MMFR0_EL1_PARANGE_4GB 0b00
#define ID_AA64MMFR0_EL1_PARANGE_64GB 0b01
#define ID_AA64MMFR0_EL1_PARANGE_1TB 0b10
#define ID_AA64MMFR0_EL1_PARANGE_4TB 0b11

/* D10.2.111 TTBR0_EL1, Translation Table Base Register 0 (EL1) */
#define TTBR0_EL1_ASID_MASK GENMASK(63, 48)
#define TTBR0_EL1_BADDR_MASK GENMASK(47, 1)
#define TTBR0_EL1_CNP BIT(0)
