# CAN Konfigürasyonunun Açıklaması (VCU)

Bu doküman, STM32 mikrodenetleyici üzerinden VCU projemiz için yapılandırılan **CAN ayarlarının ne anlama geldiğini ve neden bu şekilde seçildiğini** açıklamaktadır.

---

## Genel Hedefler

- CAN haberleşmesini düşük gecikme ve yüksek kararlılıkla gerçekleştirmek
- Yarış ortamındaki fiziksel CAN hattı hatalarına karşı toleranslı bir yapı kurmak
- Gereksiz donanım interrupt yüklerinden kaçınmak
- Tüm modüllerle aynı baud rate ile sorunsuz haberleşmek

---

## Bit Timing Ayarları

| Parametre                | Değer        | Açıklama |
|--------------------------|--------------|----------|
| **Prescaler**            | 6            | 42 MHz clock / 6 = 7 MHz → TQ süresi: 142.857 ns |
| **BS1**                  | 13 TQ        | Uzun örnekleme süresi → Jitter’a karşı daha toleranslı |
| **BS2**                  | 2 TQ         | Minimum stabil bit sonlandırma süresi |
| **Toplam TQ**            | 16           | (1 + 13 + 2) → Sağlıklı bir zamanlama aralığı |
| **Baud Rate**            | ~437.5 kbps  | Tüm cihazlar senkron olduğundan özel baud kullanılabiliyor |
| **RJW (Resync Width)**   | 2 TQ         | BS2 ≤ RJW kuralı sağlanıyor, düzgün senkronizasyon |

---

## Basic Parameters Açıklaması

| Parametre                          | Durum     | Neden Bu Şekilde? |
|------------------------------------|-----------|--------------------|
| **Time Triggered Communication**   | ❌ Disable | Sistemde zaman tabanlı iletişim protokolü (TTCAN) kullanılmıyor |
| **Automatic Bus-Off Management**   | ✅ Enable  | Bus-Off durumlarında CAN modülünün otomatik toparlanması sağlanır — yarışta kritik |
| **Automatic Wake-Up Mode**         | ❌ Disable | Sleep/Standby kullanılmadığı için gereksiz interruptlardan kaçınıldı |
| **Automatic Retransmission**       | ✅ Enable  | Veri kaybı riskini azaltır, hata durumunda CAN otomatik olarak tekrar dener |
| **Receive FIFO Locked Mode**       | ❌ Disable | FIFO dolduğunda en eski veri silinir → kayıpsız, sürekli akan veri için bu tercih edildi |
| **Transmit FIFO Priority**         | ❌ Disable | FIFO sırasına göre gönderim yapılır → önceliklendirme gerekmediği için sade yapı tercih edildi |

---

## Geliştirici Notları

- `Automatic Bus-Off Management = Enable` yapılmasa, hata sonrası `CAN_HandleTypeDef` yapısında `State = BUSY` kalabilir. Bu durumda manüel yeniden başlatma kodu yazman gerekirdi.
- `Auto Retransmission` sistemsel olarak çok önemlidir. Çünkü CAN'da **tek ACK bile alınmazsa hata sayacı artar**. Bunu donanıma bırakmak daha güvenlidir.
- Dış cihazlar (datalogger vs.) eklenecekse baud rate 500 kbps olarak tekrar ayarlanmalı ve bu yapı gözden geçirilmelidir.

---

## Özet

Bu yapılandırma ile CAN:

- Zamanlama açısından toleranslı
- Hatalara karşı dirençli
- Gereksiz donanım fonksiyonları devre dışı bırakılmış
- Yarış sırasında güvenli ve sürdürülebilir iletişim sağlayacak şekilde optimize edilmiştir.