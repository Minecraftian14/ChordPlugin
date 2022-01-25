#include "plugin.hpp"


struct Inputs : Module {
    enum ParamId {
        IN_CH_ONE_PARAM,
        IN_CH_TWO_PARAM,
        IN_CH_THR_PARAM,
        IN_CH_FOU_PARAM,
        PARAMS_LEN
    };
    enum InputId {
        INPUTS_LEN
    };
    enum OutputId {
        OUT_CH_ONE_OUTPUT,
        OUT_CH_TWO_OUTPUT,
        OUT_CH_THR_OUTPUT,
        OUT_CH_FOU_OUTPUT,
        OUTPUTS_LEN
    };
    enum LightId {
        LIGHTS_LEN
    };

    Inputs() {
        config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
        configParam(IN_CH_ONE_PARAM, -1.f, 1.f, 0.f, "");
        configParam(IN_CH_TWO_PARAM, -2.f, 2.f, 0.f, "");
        configParam(IN_CH_THR_PARAM, -10.f, 10.f, 0.f, "");
        configParam(IN_CH_FOU_PARAM, -12.f, 12.f, 0.f, "");
        configOutput(OUT_CH_ONE_OUTPUT, "");
        configOutput(OUT_CH_TWO_OUTPUT, "");
        configOutput(OUT_CH_THR_OUTPUT, "");
        configOutput(OUT_CH_FOU_OUTPUT, "");
    }

    void process(const ProcessArgs &args) override {
        outputs[OUT_CH_ONE_OUTPUT].setVoltage(params[IN_CH_ONE_PARAM].getValue());
        outputs[OUT_CH_TWO_OUTPUT].setVoltage(params[IN_CH_TWO_PARAM].getValue());
        outputs[OUT_CH_THR_OUTPUT].setVoltage(params[IN_CH_THR_PARAM].getValue());
        outputs[OUT_CH_FOU_OUTPUT].setVoltage(params[IN_CH_FOU_PARAM].getValue());
    }

};


struct InputsWidget : ModuleWidget {
    InputsWidget(Inputs *module) {
        setModule(module);
        setPanel(createPanel(asset::plugin(pluginInstance, "res/Inputs.svg")));

        addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
        addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
        addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
        addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(7.642, 11.577)), module, Inputs::IN_CH_ONE_PARAM));
        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(7.642, 38.036)), module, Inputs::IN_CH_TWO_PARAM));
        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(7.642, 64.494)), module, Inputs::IN_CH_THR_PARAM));
        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(7.642, 90.952)), module, Inputs::IN_CH_FOU_PARAM));

        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.642, 22.16)), module, Inputs::OUT_CH_ONE_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.642, 48.618)), module, Inputs::OUT_CH_TWO_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.642, 75.076)), module, Inputs::OUT_CH_THR_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.642, 101.535)), module, Inputs::OUT_CH_FOU_OUTPUT));
    }
};


Model *modelInputs = createModel<Inputs, InputsWidget>("MCXIVInputs");