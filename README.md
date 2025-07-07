# 🏎️ KOU Racing VCU Project

## Proje Tanımı

Bu repo, **Kocaeli Üniversitesi Formula Student Takımı (KOU Racing)** için geliştirilen **Vehicle Control Unit (VCU)** yazılımını içerir.  
Proje, STM32 mikrodenetleyici üzerinde çalışmakta olup; araç içi sensör verilerinin okunması, işlenmesi, CAN protokolüyle iletilmesi ve SD karta loglanması işlevlerini yerine getirir.

---

## Sistem Mimarisi

- Tüm kodlar **tek bir ana CubeMX projesi** (`vcu`) içinde çalışır.
- Her bir sensör modülü (`gyro`, `apps`, `bpps`, `speed`, `crash`) ayrı `STM32` dosyalarında `modules/` klasöründe yer alır.
- `vcu`, klasörü bu modülleri kullanarak verileri işler ve yönetir.
- **Loglama** ve **CAN ile veri iletimi**, ortak `VehicleData_t` struct'ı üzerinden gerçekleştirilir.

---

## Dosya Yapısı

```plaintext
VCU-Project/
├── vcu/           # Tüm modüllerin çağrılıp işlendiği ana dosya
│
├── modules/                    # Tüm sensör ve sistem modülleri burada
│   ├── apps_sensor.[c/h]       # Gaz pedalı sensörü (APPS)
│   ├── bpps_sensor.[c/h]       # Fren pedalı sensörü (BPPS)
│   ├── speed_sensor.[c/h]      # Hız sensörü
│   ├── gyro_sensor.[c/h]       # Gyro sensörü
│   └── crash_sensor.[c/h]      # Çarpışma sensörü
│   
│
├── docs/                       # Teknik dökümantasyon
│
├── tools/                      # Yardımcı analiz araçları
│
├── .gitignore
├── LICENSE
└── README.md                   

...