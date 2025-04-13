//ĐỒ ÁN MÔN HỌC - CƠ SỞ ĐIỆN TỬ Y SINH
//NHÓM SINH VIÊN: 22200019 - Lê Phong Cảnh ; 22200041 - Nguyễn Bảo Duy ; 22200063 - Phạm Trọng Hoàng ; 22200084 - Đặng Đình Khôi

//1. THÊM CÁC THÔNG TIN KẾT NỐI TỚI BLYNK
#define BLYNK_TEMPLATE_ID "TMPL6KM8EcDDR"                   //Mã ID của template đã tạo trên Blynk
#define BLYNK_TEMPLATE_NAME "DO AN DIEN TU Y SINH"          //Tên của template: ĐỒ ÁN ĐIỆN TỬ Y SINH
#define BLYNK_AUTH_TOKEN "UkvyF0dsimZDyjJW7Gen9efkME3zxoJM" //Mã token bảo mật để kết nối đến Server Blynk 
#define BLYNK_PRINT Serial                                  //Gửi các thông báo debug của Blynk ra Serial Monitor

//2. THÊM CÁC THƯ VIỆN CẦN THIẾT
#include <Wire.h>                   //Thư viện giao tiếp I2C với MAX30100 và OLED
#include <Ticker.h>                 //Thư viện tạo các hàm gọi ngắt (callbacks) theo khoảng thời gian
#include "MAX30100_PulseOximeter.h" //Thư viện hỗ trợ đo nhịp tim và SpO2 cho MAX30100
#include <Adafruit_GFX.h>           //Thư viện cấp đồ họa cơ bản (chữ, hình khối,...) cho OLED
#include <Adafruit_SSD1306.h>       //Thư viện cấp driver giao tiếp với OLED dùng chip SSD1306
#include <Fonts/FreeSerif9pt7b.h>   //Thư viện cấp phông chữ FreeSerif9pt7b cho kết quả hiển thị trên màn hình OLED
#include <BlynkSimpleEsp8266.h>     //Thư viện giao tiếp với Blynk IoT qua WiFi

//3. THÊM THÔNG TIN KẾT NỐI WIFI TỪ ESP8266
char ssid[] = "Evkey";              //Tên của Wifi sử dụng là Evkey
char pass[] = "12345678";           //Password của Wifi sử dụng là 12345678

//4. KHAI BÁO CÁC ĐỐI TƯỢNG VÀ KHAI BÁO BIẾN
PulseOximeter pox;                            //Khai báo đối tượng pox từ lớp PulseOximeter trong thư viện MAX30100_PulseOximeter.h để đo nhịp tim và SpO2
Adafruit_SSD1306 display(128, 32, &Wire, -1); //Khai báo đối tượng display để giao tiếp với màn hình OLED SSD1306 0.91 inch 128x32 pixel
                                              //Dùng I2C thông qua đối tượng Wire, không dùng chân Reset (đặt là -1)                                              
Ticker timer;                                 //Khai báo đối tượng timer thuộc lớp Ticker trong thư viện Ticker.h để gọi hàm định kì, không cần delay
WidgetLED led(V0);                            //Khai báo đối tượng led gán với Virtual Pin V0 trên Blynk để hiển thị trạng thái kết nối
unsigned long timeS = 0;                      //Khai báo biến timeS (ms) có kiểu dữ liệu là unsigned long để lưu thời gian
void update() {                               //Hàm gọi phương thức update của đối tượng pox từ thư viện MAX30100_PulseOximeter.h, cập nhật giá trị đo từ MAX30100
  pox.update();
}

//5. HÀM VOID SETUP()
void setup() {
  Serial.begin(115200);                       //Bật cổng Serial với tốc độ baudrate 115200 để giao tiếp với máy tính 
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);  //Kết nối ESP8266 đến Server Blynk qua Wifi
  Wire.begin();                               //Kích hoạt giao tiếp I2C để kết nối với MAX30100 và OLED

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {   //Kiểm tra kết nối đến OLED bằng địa chỉ I2C 0x3C
    Serial.println("Loi ket noi OLED!");              //Nếu lỗi thì in thông báo "Loi ket noi OLED!" và dừng chương trình (while(true))
    while (true);
  }
  display.clearDisplay();                     //Xoá sạch màn hình hiển thị
  display.setFont(&FreeSerif9pt7b);           //Đặt font chữ &FreeSerif9pt7b đã thêm từ thư viện ban đầu
  display.setTextSize(1);                     //Đặt kích cỡ chữ là 1
  display.setTextColor(WHITE);                //Đặt màu chữ là màu trắng
  display.display();                          //Hiển thị kết quả ra màn hình
  delay(2000);                                //Dừng chương trình 2000ms (2s) để màn hình hiển thị nội dung ban đầu
  
  if (!pox.begin()) {                                 //Gọi pox.begin() để khởi tạo cảm biến MAX30100
    Serial.println("Loi ket noi MAX30100!");          //Nếu lỗi thì in thông báo "Loi ket noi MAX30100!" và dừng chương trình (while(true))
    while (true);
  }
  Serial.println("Ket noi den MAX30100 thanh cong!"); //Nếu không có lỗi thì in thông báo "Ket noi den MAX30100 thanh cong!" 
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);      //Đặt cường độ dòng điện 7.6mA cho LED của MAX30100
  timer.attach_ms(100, update);                       //Gọi hàm update sau mỗi 100ms để cập nhật dữ liệu từ cảm biến
  timeS = millis();                                   //Lưu vào biến timeS thời điểm hiện tại tính từ lúc bắt đầu chương trình
}

//6. HÀM VOID LOOP() 
void loop() {
  Blynk.run();                                //Đảm bảo kết nối đến Server Blynk
  if (millis() - timeS > 1000) {              //Kiểm tra sự chênh lệch thời gian giữa thời điểm hiện tại và lần cập nhật gần nhất
                                              //Nếu hơn 1000ms (1s) thì bắt đầu lấy và xuất dữ liệu
                                              //Sau đó cập nhật lại timeS bằng thời điểm hiện tại để bắt đầu đếm cho chu kỳ tiếp theo                                        
    int heartRate = pox.getHeartRate();       //Lấy dữ liệu nhịp tim đo được (bpm)
    int spo2 = pox.getSpO2();                 //Lấy dữ liệu đo SpO2 đo được (%)
    
    //Gửi dữ liệu ra Serial Monitor theo dạng (heartRate,spo2) 
    //Gửi dữ liệu qua cổng COM4 lên Python để thực hiện phân loại tình trạng sức khoẻ 
    Serial.print(heartRate);
    Serial.print(",");
    Serial.println(spo2);
    
    //Hiển thị kết quả lên màn hình OLED
    display.fillRect(0, 0, 128, 32, BLACK);   //Xóa màn hình OLED 0.91 inch 128x32 pixel bằng 1 hình chữ nhật màu đen
    display.setCursor(0, 12);                 //Vị trí của dòng 1
    display.print("HeartR: ");                //In ra tiêu đề HeartR:
    display.print(heartRate, 0);              //In ra giá trị nhịp tim đo được
    display.println(" bpm");                  //In ra đơn vị bpm tương ứng
    display.setCursor(0, 28);                 //Vị trí của dòng 2
    display.print("SpO2: ");                  //In ra tiêu đề SpO2: 
    display.print(spo2);                      //In ra giá trị SpO2 đo được
    display.println(" %");                    //In ra đơn vị % tương ứng
    display.display();                        //Cập nhật các kết quả lên màn hình OLED
    
    // Gửi dữ liệu lên Blynk và cập nhật trạng thái led kết nối trên Blynk
    Blynk.virtualWrite(V1, heartRate);        //Gửi giá trị nhịp tim lên Virtual Pin V1 của Blynk
    Blynk.virtualWrite(V2, spo2);             //Gửi giá trị SpO2 lên Virtual Pin V2 của Blynk
    if (Blynk.connected())                 
      led.on();                               //Nếu kết nối đến Blynk thành công, led đỏ bật (on)
    else
      led.off();                              //Nếu kết nối đến Blynk không thành công, led đỏ tắt (off)
      
    timeS = millis();                         //Cập nhật lại biến thời gian timeS để thực hiện vòng lặp tiếp theo
  }
}
