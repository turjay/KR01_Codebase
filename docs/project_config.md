### STM32CubeMX Project Configuration

- **Application Structure**: `Advanced` – Daha modüler ve okunabilir kod yapısı için tercih edildi.
- **Toolchain**: `STM32CubeIDE`
- **MCU**: `STM32F407VGTx` – Yüksek hız ve çevresel destekle VCU için uygundur.
- **Firmware Version**: `STM32Cube_FW_F4 V1.28.2` (latest)
- **Heap/Stack Size**:
  - Heap: `0x200` (512 byte)
  - Stack: `0x400` (1024 byte)
- **Kod Üretim Seçenekleri**:
  - `Keep User Code` aktif – Manuel yazılan kodlar korunur
  - `Delete Unused Files` aktif – Temiz yapı
  - `Copy only necessary library files` aktif – Hafif proje yapısı
- **Free Pins** analog yapılmadı – Gelecekte GPIO olarak kullanılabilir.