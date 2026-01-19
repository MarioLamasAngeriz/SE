/*
 * ADC16 DMA Demo - Con habilitación explícita de interrupciones
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_common.h"
#include "fsl_adc16.h"
#include "fsl_dmamux.h"
#include "fsl_dma.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_ADC16_CHANNEL 8U
#define DEMO_ADC16_CHANNEL_GROUP 0U
#define DEMO_ADC16_BASEADDR ADC0
#define DEMO_DMAMUX_BASEADDR DMAMUX0
#define DEMO_DMA_CHANNEL 0U
#define DEMO_DMA_ADC_SOURCE 40U
#define DEMO_DMA_BASEADDR DMA0
#define ADC16_RESULT_REG_ADDR 0x4003b010U
#define DEMO_DMA_IRQ_ID DMA0_IRQn
#define DEMO_ADC16_SAMPLE_COUNT 16U

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_Transfer_Done = false;
volatile uint32_t g_dmaIntCount = 0;
static uint32_t g_adc16SampleDataArray[DEMO_ADC16_SAMPLE_COUNT];
static uint32_t g_avgADCValue = 0U;
dma_handle_t g_DMA_Handle;
dma_transfer_config_t g_transferConfig;
static uint32_t g_sampleCount = 0;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DMA_Configuration(void);
static void DMAMUX_Configuration(void);
static void ADC16_Configuration(void);
static void ProcessSampleData(void);
static void Delay(uint32_t count);

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Handler de interrupción DMA0 - DEBE tener este nombre exacto */
void DMA0_IRQHandler(void)
{
    g_dmaIntCount++;
    
    /* Limpiar flags de interrupción */
    DMA_ClearChannelStatusFlags(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, kDMA_TransactionsDoneFlag);
    
    /* Marcar transferencia completa */
    g_Transfer_Done = true;
    
    /* Preparar para próxima transferencia */
    DMA_PrepareTransfer(&g_transferConfig, 
                        (void *)ADC16_RESULT_REG_ADDR, sizeof(uint32_t),
                        (void *)g_adc16SampleDataArray, sizeof(uint32_t), 
                        sizeof(g_adc16SampleDataArray),
                        kDMA_PeripheralToMemory);
    
    DMA_SubmitTransfer(&g_DMA_Handle, &g_transferConfig, kDMA_EnableInterrupt);
    DMA_StartTransfer(&g_DMA_Handle);
}

int main(void)
{
    adc16_channel_config_t adcChnConfig;

    /* Desactivar Watchdog */
    SIM->COPC = 0u;

    /* Init board hardware */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    Delay(500000);

    PRINTF("\r\n");
    PRINTF("=================================\r\n");
    PRINTF("ADC16 CONTINUOUS DMA DEMO v2\r\n");
    PRINTF("=================================\r\n");

    /* Habilitar clock del DMA */
    CLOCK_EnableClock(kCLOCK_Dma0);
    CLOCK_EnableClock(kCLOCK_Dmamux0);

    /* Inicializar periféricos */
    DMAMUX_Configuration();
    DMA_Configuration();
    
    /* CRÍTICO: Habilitar interrupción DMA0 en NVIC */
    NVIC_SetPriority(DMA0_IRQn, 2);
    NVIC_EnableIRQ(DMA0_IRQn);
    
    ADC16_Configuration();

    /* Configurar canal ADC */
    adcChnConfig.channelNumber = DEMO_ADC16_CHANNEL;
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adcChnConfig.enableDifferentialConversion = false;
#endif
    adcChnConfig.enableInterruptOnConversionCompleted = false;
    
    PRINTF("\r\n");
    PRINTF("DMA and ADC initialized\r\n");
    PRINTF("Light sensor: ADC Channel %d\r\n", DEMO_ADC16_CHANNEL);
    PRINTF("Reading samples automatically every 2 seconds...\r\n");
    PRINTF("\r\n");
    
    /* Loop infinito */
    while (1)
    {
        /* Resetear flag */
        g_Transfer_Done = false;
        
        PRINTF("Starting ADC conversion... ");
        
        /* Iniciar conversión ADC */
        ADC16_SetChannelConfig(DEMO_ADC16_BASEADDR, DEMO_ADC16_CHANNEL_GROUP, &adcChnConfig);
        
        /* Esperar a que DMA complete */
        uint32_t timeout = 5000000;
        while (!g_Transfer_Done && timeout > 0)
        {
            timeout--;
        }
        
        if (timeout == 0)
        {
            PRINTF("TIMEOUT!\r\n");
            PRINTF("  DMA interrupts received: %d\r\n", g_dmaIntCount);
            PRINTF("  Transfer done flag: %d\r\n", g_Transfer_Done);
            
            /* Verificar estado del DMA */
            uint32_t dma_status = DMA0->DMA[DEMO_DMA_CHANNEL].DSR_BCR;
            PRINTF("  DMA Status Register: 0x%08X\r\n", dma_status);
            
            /* Reintentar */
            PRINTF("  Reinitializing DMA...\r\n");
            DMA_Configuration();
        }
        else
        {
            ProcessSampleData();
            g_sampleCount++;
            
            PRINTF("OK\r\n");
            PRINTF("  Sample #%d: %5d (0x%04X) - DMA ints: %d\r\n", 
                   g_sampleCount, g_avgADCValue, g_avgADCValue, g_dmaIntCount);
        }
        
        /* Esperar antes de siguiente lectura */
        Delay(2000000);
    }
}

static void Delay(uint32_t count)
{
    for(volatile uint32_t i = 0; i < count; i++)
    {
        __NOP();
    }
}

static void DMAMUX_Configuration(void)
{
    DMAMUX_Init(DEMO_DMAMUX_BASEADDR);
    DMAMUX_SetSource(DEMO_DMAMUX_BASEADDR, DEMO_DMA_CHANNEL, DEMO_DMA_ADC_SOURCE);
    DMAMUX_EnableChannel(DEMO_DMAMUX_BASEADDR, DEMO_DMA_CHANNEL);
}

static void DMA_Configuration(void)
{
    /* Inicializar DMA */
    DMA_Init(DEMO_DMA_BASEADDR);
    
    /* Crear handle */
    DMA_CreateHandle(&g_DMA_Handle, DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL);
    
    /* NO usar DMA_SetCallback - vamos a manejar la interrupción directamente */
    
    /* Preparar transferencia */
    DMA_PrepareTransfer(&g_transferConfig, 
                        (void *)ADC16_RESULT_REG_ADDR, sizeof(uint32_t),
                        (void *)g_adc16SampleDataArray, sizeof(uint32_t), 
                        sizeof(g_adc16SampleDataArray),
                        kDMA_PeripheralToMemory);
    
    /* Enviar configuración */
    DMA_SubmitTransfer(&g_DMA_Handle, &g_transferConfig, kDMA_EnableInterrupt);
    
    /* Iniciar */
    DMA_StartTransfer(&g_DMA_Handle);
}

static void ADC16_Configuration(void)
{
    adc16_config_t adcUserConfig;

    /* Habilitar clock del ADC */
    CLOCK_EnableClock(kCLOCK_Adc0);

    ADC16_GetDefaultConfig(&adcUserConfig);
    adcUserConfig.resolution = kADC16_Resolution16Bit;
    adcUserConfig.enableContinuousConversion = true;
    adcUserConfig.clockSource = kADC16_ClockSourceAlt1;
    adcUserConfig.longSampleMode = kADC16_LongSampleCycle24;
    adcUserConfig.enableLowPower = false;
    
#if ((defined BOARD_ADC_USE_ALT_VREF) && BOARD_ADC_USE_ALT_VREF)
    adcUserConfig.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
#endif
    
    ADC16_Init(DEMO_ADC16_BASEADDR, &adcUserConfig);

#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
    PRINTF("Calibrating ADC...\r\n");
    if (kStatus_Success == ADC16_DoAutoCalibration(DEMO_ADC16_BASEADDR))
    {
        PRINTF("ADC calibration: OK\r\n");
    }
    else
    {
        PRINTF("ADC calibration: FAILED\r\n");
    }
#endif

    ADC16_EnableHardwareTrigger(DEMO_ADC16_BASEADDR, false);
    ADC16_EnableDMA(DEMO_ADC16_BASEADDR, true);
}

static void ProcessSampleData(void)
{
    uint32_t sum = 0;

    for (uint32_t i = 0; i < DEMO_ADC16_SAMPLE_COUNT; i++)
    {
        sum += g_adc16SampleDataArray[i];
    }
    
    g_avgADCValue = sum / DEMO_ADC16_SAMPLE_COUNT;

    /* Limpiar buffer */
    for (uint32_t i = 0; i < DEMO_ADC16_SAMPLE_COUNT; i++)
    {
        g_adc16SampleDataArray[i] = 0U;
    }
}
