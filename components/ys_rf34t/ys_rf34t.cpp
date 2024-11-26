#include "esphome/core/log.h"
#include "ys_rf34t.h"

namespace esphome
{
  namespace ys_rf34t
  {

    static const char *TAG = "ys_rf34t.component";

    void YSRF34TUARTComponent::setup()
    {
    }

    void YSRF34TUARTComponent::loop()
    {
      // const int max_line_length = 80;
      // static char buffer[max_line_length];
      while (available())
      {
        uint8_t header;
        read_byte(&header); // 读取一个字节作为消息头

        // 检查消息头是否为 0xFD，表示消息开始
        if (header == 0xFD)
        {
          // 开始读取数据字段
          uint8_t emit_time;
          uint8_t addr2;
          uint8_t addr1;
          uint8_t key_code;
          uint8_t osc_param;
          uint8_t end;

          // 依次读取数据字段
          read_byte(&emit_time); // 读取 emit_time 字段
          read_byte(&addr2);     // 读取 addr2 字段
          read_byte(&addr1);     // 读取 addr1 字段
          read_byte(&key_code);  // 读取 key_code 字段
          read_byte(&osc_param); // 读取 osc_param 字段
          read_byte(&end);       // 读取结束标志字段

          // 创建 YSRF34TData 实例并赋值

          // 检查结束标志是否为 0xDF，表示消息结束
          if (end == 0xDF)
          {
            // 创建 YSRF34TData 实例并赋值
            YSRF34TData data;
            data.emit_time = emit_time;
            data.addr2 = addr2;
            data.addr1 = addr1;
            data.key_code = key_code;
            data.osc_param = osc_param;

            // 记录接收到的 RF 消息的调试信息
            ESP_LOGD(TAG, "Received rf message: emit_time=0x%02X, addr2=0x%02X, addr1=0x%02X, key_code=0x%02X, osc_param=0x%02X",
                     data.emit_time, data.addr2, data.addr1, data.key_code, data.osc_param);
            // 调用数据回调函数处理接收到的数据

            this->data_callback_.call(data);
            continue; // 继续循环，处理下一个可用数据
          }
          ESP_LOGE("Invalid end code! Not valid rf message."); // 记录错误日志，表示无效的 RF 消息
        }
      }
    }

    void YSRF34TUARTComponent::dump_config()
    {
      ESP_LOGCONFIG(TAG, "Empty UART component");
    }

    void YSRF34TUARTComponent::send_rf(YSRF34TData data)
    {
      // 记录调试信息，输出 RF 消息的各个字段，以16进制格式显示
      ESP_LOGD(TAG, "Sending rf message: emit_time=0x%02X, addr2=0x%02X, addr1=0x%02X, key_code=0x%02X, osc_param=0x%02X",
               data.emit_time, data.addr2, data.addr1, data.key_code, data.osc_param);

      // 发送数据包的开始标志 0xFD
      write_byte(0xFD);

      // 依次发送数据字段
      write_byte(data.emit_time); // 发送 emit_time 字段
      write_byte(data.addr2);     // 发送 addr2 字段
      write_byte(data.addr1);     // 发送 addr1 字段
      write_byte(data.key_code);  // 发送 key_code 字段
      write_byte(data.osc_param); // 发送 osc_param 字段

      // 发送数据包的结束标志 0xDF
      write_byte(0xDF);
    }

  } // namespace empty_uart_component
} // namespace esphome