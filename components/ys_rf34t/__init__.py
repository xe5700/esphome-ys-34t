import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import (
    CONF_ID,
    CONF_TRIGGER_ID,
)
from esphome import automation

DEPENDENCIES = ["uart"]
CODEOWNERS = ["@xe5700"]
CONF_ALLOW_SEND_RF = "allow_send_rf"
ys_rf34t_ns = cg.esphome_ns.namespace("ys_rf34t")
YSRF34TUARTComponent = ys_rf34t_ns.class_(
    "YSRF34TUARTComponent", cg.Component, uart.UARTDevice
)
YS34TData = ys_rf34t_ns.struct("YSRF34TData")
YSRF34TReceivedCodeTrigger = ys_rf34t_ns.class_(
    "YSRF34TReceivedCodeTrigger", automation.Trigger.template(YS34TData)
)
CONF_ON_CODE_RECEIVED = "on_code_received"
CONFIG_SCHEMA = (
    cv.Schema({
        cv.GenerateID(): cv.declare_id(YSRF34TUARTComponent),
        cv.Optional(CONF_ALLOW_SEND_RF): cv.boolean,
            cv.Optional(CONF_ON_CODE_RECEIVED): automation.validate_automation(
                {
                    cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(
                        YSRF34TReceivedCodeTrigger
                    ),
                }
            ),
    })
    .extend(cv.COMPONENT_SCHEMA)
    .extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
    
    for conf in config.get(CONF_ON_CODE_RECEIVED, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [(YS34TData, "data")], conf)


YSRF34TSendRfAction = ys_rf34t_ns.class_(
    "YSRF34TSendRfAction", automation.Action
)
YSRF34T_ID_SCHEMA = cv.Schema({cv.GenerateID(): cv.use_id(YSRF34TUARTComponent)})
# 定义 CONF_ 变量，值为相应的字段名称
CONF_EMIT_TIME = "emit_time"
CONF_ADDR2 = "addr2"
CONF_ADDR1 = "addr1"
CONF_KEY_CODE = "key_code"
CONF_OSC_PARAM = "osc_param"
CONF_END = "end"

@automation.register_action("ys_rf34t.send_rf", YSRF34TSendRfAction, YSRF34T_ID_SCHEMA)
async def ys_rf34t_send_rf_to_rf(config, action_id,template_args, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_args, paren)

    # 使用 YSRF34TData 中的字段名称和类型
    template_ = await cg.templatable(config[CONF_EMIT_TIME], args, cg.uint8)
    cg.add(var.set_emit_time(template_))

    template_ = await cg.templatable(config[CONF_ADDR2], args, cg.uint8)
    cg.add(var.set_addr2(template_))

    template_ = await cg.templatable(config[CONF_ADDR1], args, cg.uint8)
    cg.add(var.set_addr1(template_))

    template_ = await cg.templatable(config[CONF_KEY_CODE], args, cg.uint8)
    cg.add(var.set_key_code(template_))

    template_ = await cg.templatable(config[CONF_OSC_PARAM], args, cg.uint8)
    cg.add(var.set_osc_param(template_))
    return var