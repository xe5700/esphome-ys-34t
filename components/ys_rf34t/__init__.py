import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

DEPENDENCIES = ["uart"]
CONF_ALLOW_SEND_RF = "allow_send_rf"
ys_rf34t_component_ns = cg.esphome_ns.namespace("ys_rf34t")
YSRF34TUARTComponent = ys_rf34t_component_ns.class_(
    "YSRF34TUARTComponent", cg.Component, uart.UARTDevice
)

CONFIG_SCHEMA = (
    cv.Schema({
        cv.GenerateID(): cv.declare_id(YSRF34TUARTComponent),
        cv.Optional(CONF_ALLOW_SEND_RF): cv.boolean
    })
    .extend(cv.COMPONENT_SCHEMA)
    .extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
