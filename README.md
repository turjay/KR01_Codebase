# 🏎️ KOU Racing VCU Project

## Proje Tanımı

Bu repo, **Kocaeli Üniversitesi Formula Student Takımı (KOU Racing)** için geliştirilen **Vehicle Control Unit (VCU)** yazılımını içerir.  
Proje, STM32 mikrodenetleyici üzerinde çalışmakta olup; araç içi sensör verilerinin okunması, işlenmesi, CAN protokolüyle iletilmesi ve SD karta loglanması işlevlerini yerine getirir.

## Sistem Mimarisi

- Tüm kodlar **tek bir ana CubeMX projesi** (`vcu`) içinde çalışır.
- Her bir sensör modülü ayrı `STM32` dosyalarında `modules/` klasöründe yer alır.
- `vcu`, klasörü bu modülleri kullanarak verileri işler ve yönetir.

## Dosya Yapısı

```plaintext
VCU-Project/
├── vcu/                       # Modüller VCU klasörüne taşımadan önce test ettiğimiz klasör
│
├── test/                       # Modüller VCU klasörüne taşımadan önce test ettiğimiz klasör
│   ├── apps_sensor.[c/h]       
│   ├── bpps_sensor.[c/h]       
│   ├── crash_sensor.[c/h]      
│   ├── gyro_sensor.[c/h]
│   ├── nextion_screen.[c/h]
│   ├── ready2drive.[c/h]   
│   └── steering_sensor.[c/h]
│
├── docs/                       # Sensörlerin ve bizim dokümantasyonlarımız
│
├── tools/                      # Yardımcı araçlar
│
├── .gitignore
├── LICENSE
└── README.md
```

## To-do list

- [X] Buzzer - *Batu*
- [X] R2D - *Türkay*
- [ ] VCU - *Türkay & Batu*
- [X] Gyro - *Türkay*
- [X] BMS - *Batu*
- [ ] BPPS - *Türkay*
---
- [ ] Ekran -  *Mert*
- [X] APPS - *Kerim*
- [ ] Hız - *Mert*
