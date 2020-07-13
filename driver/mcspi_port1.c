#include "mcspi_port1.h"

void McSPI1AintcConfigure(IntHandler handler, uint32_t priority) {
    IntRegister(SYS_INT_SPI1INT, *(handler));
    IntPrioritySet(SYS_INT_SPI1INT, priority, AINTC_HOSTINT_ROUTE_IRQ);
    IntSystemEnable(SYS_INT_SPI1INT);
}

void McSPI1SetUp(McSPI_Channel channel) {
	McSPIReset(SOC_SPI_1_REGS);
    McSPICSEnable(SOC_SPI_1_REGS);
    McSPIMasterModeEnable(SOC_SPI_1_REGS);
    McSPIMasterModeConfig(SOC_SPI_1_REGS, MCSPI_SINGLE_CH, MCSPI_TX_ONLY_MODE, MCSPI_DATA_LINE_COMM_MODE_6, channel);
    McSPIClkConfig(SOC_SPI_1_REGS, MCSPI_IN_CLK, MCSPI1_OUT_FREQ, channel, MCSPI_CLK_MODE_0);
    McSPIWordLengthSet(SOC_SPI_1_REGS, MCSPI_WORD_LENGTH(8), channel);
    McSPICSPolarityConfig(SOC_SPI_1_REGS, MCSPI_CS_POL_LOW, channel);
    McSPITxFIFOConfig(SOC_SPI_1_REGS, MCSPI_TX_FIFO_ENABLE, channel);
    McSPIRxFIFOConfig(SOC_SPI_1_REGS, MCSPI_RX_FIFO_ENABLE, channel);
}

void McSPI1PinMuxSetup() {
	HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI1_SCLK) = (CONTROL_CONF_MUXMODE(3) |
			CONTROL_CONF_RXACTIVE | CONTROL_CONF_PULLUDEN ) & (~CONTROL_CONF_PULLUPSEL);

	HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI1_D0) = (CONTROL_CONF_MUXMODE(3) |
			CONTROL_CONF_RXACTIVE | CONTROL_CONF_PULLUDEN ) & (~CONTROL_CONF_PULLUPSEL);

	HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI1_D1) = (CONTROL_CONF_MUXMODE(3) |
			CONTROL_CONF_RXACTIVE | CONTROL_CONF_PULLUDEN ) & (~CONTROL_CONF_PULLUPSEL);
}

/**
 * @brief setup mcspi1 cs pin
 * @brief McSPI_Channel_0 address 0x99c
 * @brief McSPI_Channel_1 address 0x97c
 * */
void McSPI1CSPinMuxSetup(McSPI_Channel channel) {
	switch(channel) {
	case McSPI_Channel_0:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI1_CS0) = (CONTROL_CONF_MUXMODE(3) |
				CONTROL_CONF_RXACTIVE | CONTROL_CONF_PULLUDEN ) & (~CONTROL_CONF_PULLUPSEL);
		break;
	case McSPI_Channel_1:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI1_CS1) = (CONTROL_CONF_MUXMODE(4) |
				CONTROL_CONF_RXACTIVE | CONTROL_CONF_PULLUDEN ) & (~CONTROL_CONF_PULLUPSEL);
		break;
	default:
		break;
	}
}

/**
 * @brief This function will configure the required clocks for McSPI1 instance.
 */
void McSPI1ModuleClkConfig() {
	// set L4 clk
    HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) =
                             CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
     CM_PER_L3S_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) =
                             CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
     CM_PER_L3_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) =
                             CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
                               CM_PER_L3_INSTR_CLKCTRL_MODULEMODE) !=
                                   CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) =
                             CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) &
        CM_PER_L3_CLKCTRL_MODULEMODE) != CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) =
                             CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
                              CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL) !=
                                CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) =
                             CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) &
                             CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL) !=
                               CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKCTRL) =
                             CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKCTRL) &
      CM_PER_L4LS_CLKCTRL_MODULEMODE) != CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE);

    // set mcspi1 clk，this config can use as same as mcspi0
    HWREG(SOC_CM_PER_REGS + CM_PER_SPI1_CLKCTRL) &= ~CM_PER_SPI1_CLKCTRL_MODULEMODE;

    HWREG(SOC_CM_PER_REGS + CM_PER_SPI1_CLKCTRL) |=
                             CM_PER_SPI1_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_SPI1_CLKCTRL) &
      CM_PER_SPI1_CLKCTRL_MODULEMODE) != CM_PER_SPI1_CLKCTRL_MODULEMODE_ENABLE);

    // set L3 clk
    while(!(HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
            CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK));

    while(!(HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
            CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK));

    while(!(HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
           (CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK |
            CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L4_GCLK)));

    while(!(HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) &
           (CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_L4LS_GCLK |
            CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_SPI_GCLK)));
}