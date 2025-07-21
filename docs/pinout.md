🧩 GENEL ioc YAPILANDIRMASI
🔧 ADC Ayarları
✅ ADC1 → APPS (2 kanal)
    PA0 (ADC1_IN0)
    PA1 (ADC1_IN1)
    Continuous Conversion Mode: ENABLE
    DMA: ENABLE, DMA2_Stream4
    DMA_CIRCULAR, WORD hizalama
    NbrOfConversion: 2
✅ ADC2 → GYRO (2 kanal)
    PA4 (ADC2_IN4)
    PA5 (ADC2_IN5)
    Continuous Conv. Mode: ENABLE
    DMA: ENABLE, DMA2_Stream2
    DMA_CIRCULAR, WORD hizalama
    NbrOfConversion: 2
✅ ADC3 → BPPS (2 kanal)
    PC0 (ADC3_IN10)
    PC1 (ADC3_IN11)
    Continuous Conv. Mode: ENABLE
    DMA: ENABLE, DMA2_Stream0
    DMA_CIRCULAR, WORD hizalama
    NbrOfConversion: 2
🔧 USART2 (Ekran için UART)
    TX: PA2, RX: PA3
    Mode: Asynchronous
    Baudrate: 9600
    Pull-up: PA3 için PULLUP, PA2 için NOPULL
🔧 CAN1 (Libat / CAN haberleşmesi)
    CAN_RX: PB8, CAN_TX: PB9
    Mode: Normal
    Speed: 500 kbps önerilir ama ayarlanabilir
    Otobaud: Kapalı
    Filtre: ID mask veya listeye göre ayarlanır
🔧 GPIO (Araç hızı için 4 giriş)
    GPIO Input (No Pull veya Pull-up):
        PB0, PB1, PB10, PB11
        External interrupt özelliği istersen GPIO_EXTI yapılabilir.
    Alternatif olarak timer capture da yapılabilir (istersen söyler ayarlarım).
🔧 TIM3 (PWM çıkışı, APPS kontrol için)
    PA6 → TIM3_CH1
    Prescaler: 83
    Period: 49 → ~40kHz
    Mode: PWM Generation CH1
🔧 TIM4 (Zamanlayıcı)
    Prescaler: 8399
    Period: 0xFFFF
    1ms zaman tabanı üretmek için kullanılabilir (HAL_GetTick() alternatifi)
🔧 Saat Kaynağı
    HSE: Harici kristal 25MHz
    SYSCLK: 168MHz (PLL: M=8, N=336, P=2)
    APB1: 42MHz, APB2: 84MHz