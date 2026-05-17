# システム全体の概要
minirobo_sender側に記載  
# minirobo2026-esp.inoについて
他Readmeに記載のあった無線通信用マイコンのコードです。
esp-wroom-32を用いて、PCのreceiverからのUDP通信を受け取りI2C通信を用いてminirobo2026.1が書き込まれたArduinoNanoにデータを送信します。  
# minirobo2026.01_v0.2.inoについて
esp-wroom-32からI2C通信で受信したデータを
