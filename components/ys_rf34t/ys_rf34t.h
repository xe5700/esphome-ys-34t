#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

namespace esphome
{
  namespace ys_rf34t
  {

    class YSRF34TUARTComponent : public uart::UARTDevice, public Component
    {
    public:
      void setup() override;
      void loop() override;
      void dump_config() override;
      void send_rf(YSRF34TData data);
        void add_on_code_received_callback(std::function<void(YSRF34TData)> callback) {
          this->data_callback_.add(std::move(callback));
        }
    protected:
      CallbackManager<void(YSRF34TData)> data_callback_;
    };

    struct YSRF34TData
    {
      uint8_t emit_time;
      uint8_t addr2;
      uint8_t addr1;
      uint8_t key_code;
      uint8_t osc_param;
    };
  class YSRF34TReceivedCodeTrigger : public Trigger<YSRF34TData> {
  public:
    explicit YSRF34TReceivedCodeTrigger(YSRF34TUARTComponent *parent) {
      parent->add_on_code_received_callback([this](YSRF34TData data) { this->trigger(data); });
    }
  };
  template<typename... Ts> class YSRF34TSendCodeAction : public Action<Ts...> {
 public:
  YSRF34TSendCodeAction(YSRF34TUARTComponent *parent) : parent_(parent) {}
      TEMPLATABLE_VALUE(uinit8_t, emit_time)
      TEMPLATABLE_VALUE(uinit8_t, addr2)
      TEMPLATABLE_VALUE(uinit8_t, addr1)
      TEMPLATABLE_VALUE(uinit8_t, key_code)
      TEMPLATABLE_VALUE(uinit8_t, osc_param)
  void play(Ts... x) {
    YSRF34TData data{};
    data.emit_time = this->emit_time_.value(x...);
    data.addr2 = this->addr2_.value(x...);
    data.addr1 = this->addr1_.value(x...);
    data.key_code = this->key_code_.value(x...);
    data.osc_param = this->osc_param_.value(x...);
    this->parent_->send_rf(data);
  }

 protected:
  YSRF34TUARTComponent *parent_;
};

  } // namespace empty_uart_component
} // namespace esphome