# 🏎️ KOU Racing VCU Project

## Proje Tanımı

**STM32F407** tabanlı bir araç kontrol ünitesi projesidir. Bu proje, KOU Racing Formula Student takımının araç içi sensör verilerini toplayan, işleyen ve diğer sistemlerle haberleştiren modüler bir VCU mimarisi sunar.

---

## Dosya Yapısı

```plaintext
KOURacingVCU/
│
├── docs/ # Dokümantasyon ve teknik belgeler
│
├── test/ # Modüllerin test edildiği bağımsız test yapısı
│ ├── apps_sensor/
│ ├── bpps_sensor/
│ ├── crash_sensor/
│ ├── gyro_sensor/
│ ├── speed_sensor/
│ └── steering_sensor/
│
├── tools/ # Scriptler, yardımcı araçlar
│
├── vcu/ # Asıl proje, tüm modüllerin entegre hali
│
├── Core/
│ ├── Src/Inc # main.c, system init vs.
│ └── Modules/
│   └── Apps/
│     └── Src/Inc
│   └── Bpps/
│     └── Src/Inc
│   └── Crash/
│     └── Src/Inc
│   └── Gyro/
│     └── Src/Inc
│   └── Speed/
│     └── Src/Inc
│   └── Steering/
│     └── Src/Inc
├── Drivers/ # HAL ve CMSIS sürücüleri
├── Middlewares/ # USB Host 
└── USB_HOST/ # USB yapılandırma dosyalar
```

## 🔧 Geliştirme Ortamı

- **IDE:** STM32CubeIDE  
- **MCU:** STM32F407VG
- **Version Control:** Github

## 🧩 Modüller

| Modül          | Açıklama                                      |
|----------------|-----------------------------------------------|
| Apps           | Gaz pedalı konumu (ADC ile okunur)           |
| Bpps           | Fren pedalı güvenliği                         |
| Crash          | Çarpışma sensörü / shutoff logic             |
| Gyro           | Gyrosensör (CAN üzerinden veri okuma)        |
| Speed          | Hız sensörü (gelecek sürümde)                |
| Steering       | Direksiyon açı sensörü (ADC/CAN)             |

Her modül CubeMX üzerinden ayrı ayrı yapılandırılıp kendi `Inc/` ve `Src/` dizininde yazılmıştır.
