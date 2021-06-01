// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore DDK v 1.0
// Date : 

#pragma once
#define WD_DRIVER_NAME_CHANGE
#include "utils.h"
#include "wdc_defs.h"
#include "windrvr.h"
#include "windrvr_int_thread.h"
#include "windrvr_events.h"
#pragma comment(lib, "wdapi1011.lib")

/*************************************************************
  General definitions
 *************************************************************/

// Interrupt result information struct
typedef struct
{
    DWORD dwCounter;						// Number of interrupts received
    DWORD dwLost;							// Number of interrupts not yet handled
    WD_INTERRUPT_WAIT_RESULT waitResult;	// See WD_INTERRUPT_WAIT_RESULT values in windrvr.h
    DWORD dwEnabledIntType;					// Interrupt type that was actually enabled
											// (MSI/MSI-X/Level Sensitive/Edge-Triggered)
    DWORD dwLastMessage;					// Message data of the last received MSI/MSI-X
											// (Windows Vista/7); N/A to line-based interrupts)
} PCIe_INT_RESULT;
/* TODO: You can add fields to DEMO_INT_RESULT to store any additional
         information that you wish to pass to your diagnostics interrupt
         handler routine (DiagIntHandler() in demo_diag.c) */

typedef void (*PCIe_INT_HANDLER)(WDC_DEVICE_HANDLE hDev, PCIe_INT_RESULT *pIntResult);		// DEMO diagnostics interrupt handler function type
typedef void (*PCIe_EVENT_HANDLER)(WDC_DEVICE_HANDLE hDev, DWORD dwAction);					// DEMO diagnostics plug-and-play and power management events handler function type

BOOL PCIeInterruptEnable(PCIe_INT_HANDLER funcIntHandler);
void PCIeInterruptDisable(void);
BOOL PCIeEventRegister(PCIe_EVENT_HANDLER funcEventHandler);
void PCIeEventUnregister(void);

//UINT64 TranslatePhysical(WDC_DEVICE_HANDLE hDev, int bar, int address);

typedef struct{
	WDC_DEVICE_HANDLE		hDev;
	WD_PCI_CARD_INFO		Info;
}PCIeCard;

typedef struct{ // common config : 0x000
	WORD		VID;		// vendor id
	WORD		DID;		// device id

	// Offset 04h : command register
	struct{
		DWORD	: 2;
		DWORD	bus_master_enable				: 1;
		DWORD	special_cycle_enable			: 1;
		DWORD	memory_write_and_invalidate		: 1;
		DWORD	vga_palette_snoop				: 1;
		DWORD	parity_error_enable				: 1;
		DWORD	idsel_stepping_wait_cycle_ctrl	: 1;
		DWORD	serr_enable						: 1;
		DWORD	fast_back_to_back_transaction_enable	: 1;
		DWORD	interrupt_disabe				: 1;
		DWORD	: 5;
	}command;
	WORD		status;

	struct{
		BYTE		Revision_ID;
		DWORD		class_code	: 24;
	};
	BYTE		cache_line_size;
	BYTE		master_latency_timer;
	BYTE		header_type;
	BYTE		BIST;
}REG_PCIE_COMMON;

typedef struct{	// config address : 0x080
	// Offset 00h
	BYTE			pcie_cap_id;
	// Offset 01h
	BYTE			next_cap_pointer;
	// Offset 02h
	struct{
		DWORD		version		: 4;		// pcie version : 1(pcie 1.0), 2(pcie 2.0)
		DWORD		type		: 4;		// Device/Port Type : 0000b(PCIe endpoint device), 0001b(Lagacy PCI Express endpoint device), 0100b(Root Port of PCI Express Root Complex), ....
		DWORD		slot_imp	: 1;		// slot implemented	: this fied is valid for the following PCI Express device/Port types : root port of PCI Express root complex, downstream port of PCI Express switch
		DWORD		int_mes_num	: 5;		// interrupt message number
		DWORD					: 2;		// 
	}pcie_capabilities;
	// Offset 04h
	struct{
		DWORD		max_payload_size_support				: 3;	// 000b(128 bytes), 001b(256 bytes), 010b(512 bytes), 011b(1024 bytes), 100b(2048 bytes), 101b(4096 bytes)
		DWORD		phantom_func_support					: 2;
		DWORD		extended_tag_field_support				: 1;
		DWORD		endpoint_L0s_acceptable_latency			: 3;
		DWORD		endpoint_L1s_acceptable_latency			: 3;
		DWORD		attention_button_present				: 1;
		DWORD		attention_indicator_present				: 1;
		DWORD		power_indicator_present					: 1;
		DWORD												: 3;
		DWORD		captured_slot_power_limit_value			: 8;
		DWORD		captured_slot_power_limit_scale			: 2;
		DWORD												: 4;
	}device_capabilities;
	// Offset 08h : device control register
	struct{
		DWORD		correctable_error_reporting_enable		: 1;
		DWORD		non_fatal_error_reporting_enable		: 1;
		DWORD		fatal_error_reporting_enable			: 1;
		DWORD		unsupported_request_reporting_enable	: 1;
		DWORD		enable_relaxed_ordering					: 1;
		DWORD		max_payload_size						: 3;	// same as device_capabilities.max_payload_size_support
		DWORD		extended_tag_field_enable				: 1;
		DWORD		phantom_functions_enable				: 1;
		DWORD		aux_power_pm_enable						: 1;
		DWORD		enable_snoop_not_required				: 1;
		DWORD		max_read_request_size					: 3;
		DWORD												: 1;	// same as device_capabilities.max_payload_size_support
	}device_control;
	// Offset 0Ah : device status register
	struct{
		DWORD		correctable_error_detected				: 1;
		DWORD		non_fatal_error_detected				: 1;
		DWORD		fatal_error_detected					: 1;
		DWORD		unsupported_request_detected			: 1;
		DWORD		aux_power_detected						: 1;
		DWORD		transactions_pending					: 1;
		DWORD												: 10;
	}device_status;
	// Offset 0Ch : link capabilities register
	struct{
		DWORD		maximum_link_speed						: 4;
		DWORD		maximum_link_width						: 6;
		DWORD		aspm_support							: 2;
		DWORD		L0s_exit_latency						: 3;
		DWORD		L1_exit_latency							: 3;
		DWORD												: 6;
		DWORD		port_number								: 8;
	}link_capabilities;
	// Offset 10h : link control register
	struct{
		DWORD		aspm_control							: 2;
		DWORD												: 1;
		DWORD		read_completion_boundary				: 1;
		DWORD		link_disable							: 1;
		DWORD		retrain_link							: 1;
		DWORD		common_clock_configuration				: 1;
		DWORD		extended_synch							: 1;
		DWORD												: 8;
	}link_control;
	// Offset 12h : link status register
	struct{
		DWORD		link_speed								: 4;
		DWORD		negotiated_link_width					: 6;
		DWORD		link_training_error						: 1;
		DWORD		link_training							: 1;
		DWORD		slot_clock_configuration				: 1;
		DWORD												: 3;
	}link_status;
	// Offset 14h : slot capabilities register
	DWORD	slot_caabilities;

}REG_PCIE_CAPABILITY;

class PCIeDriver{
public:
	PCIeDriver(void);
	virtual ~PCIeDriver(void);

	BOOL Initialize(DWORD dwVendorId, DWORD dwDeviceId);
	void Release(void);
	BOOL IsInitialized(void);

	BOOL SetCurrentCard(DWORD dwCard);
	BOOL WriteConfigRegister(DWORD dwOffset, PVOID pData, DWORD dwByteSize);
	BOOL ReadConfigRegister(DWORD dwOffset, PVOID pData, DWORD dwByteSize);
	
	BOOL InterruptEnable(PCIe_INT_HANDLER funcIntHandler);
	void InterruptDisable(void);

	void* GetUserPointer(int bar, int address);
	UINT64 GetPhyAddress(int bar, int address = 0);
	DWORD Read32(DWORD dwBar, DWORD dwAddr);
	void Write32(DWORD dwBar, DWORD dwAddr, DWORD dwData);
 	void BlockRead(DWORD dwBar, DWORD dwAddr, PVOID pData, DWORD dwByteSize);
	void BlockWrite(DWORD dwBar, DWORD dwAddr, PVOID pData, DWORD dwByteSize);
	void BlockReadFIFO32(DWORD dwBar, DWORD dwAddr, PVOID pData, DWORD dwByteSize);
	void BlockWriteFIFO32(DWORD dwBar, DWORD dwAddr, PVOID pData, DWORD dwByteSize);
	void BlockReadFIFO64(DWORD dwBar, DWORD dwAddr, PVOID pData, DWORD dwByteSize);
	void BlockWriteFIFO64(DWORD dwBar, DWORD dwAddr, PVOID pData, DWORD dwByteSize);

	inline DWORD GetCardCount(void)		{return m_dwCardCount;}
	inline DWORD GetCurrentCardID(void)	{return m_dwCurrenCardID;}
	inline WDC_DEVICE_HANDLE GetDeviceHandle(void)		{return m_pCurrentCard->hDev;}

	static inline PCIeDriver* GetCurrentDriver(void)	{return m_pPCIeDriver;}

protected:
	static PCIeDriver*		m_pPCIeDriver;

	PCIeCard*				m_pCard;
	PCIeCard*				m_pCurrentCard;
	WD_PCI_CONFIG_DUMP		m_PCIeConfig;

	HANDLE					m_hWindriver;

private:
	DWORD					m_dwCardCount;
	DWORD					m_dwCurrenCardID;
	
	BOOL					m_bInitialized;
};
