#include "plugin.hpp"


struct Amp : Module {
	enum ParamId {
		PARAMS_LEN
	};
	enum InputId {
		IN_CH_ONE_INPUT,
		IN_CH_TWO_INPUT,
		IN_CH_THR_INPUT,
		IN_CH_FOU_INPUT,
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

	Amp() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configInput(IN_CH_ONE_INPUT, "");
		configInput(IN_CH_TWO_INPUT, "");
		configInput(IN_CH_THR_INPUT, "");
		configInput(IN_CH_FOU_INPUT, "");
		configOutput(OUT_CH_ONE_OUTPUT, "");
		configOutput(OUT_CH_TWO_OUTPUT, "");
		configOutput(OUT_CH_THR_OUTPUT, "");
		configOutput(OUT_CH_FOU_OUTPUT, "");
	}

	void process(const ProcessArgs& args) override {
        outputs[OUT_CH_ONE_OUTPUT].setVoltage(inputs[IN_CH_ONE_INPUT].getVoltage());
        outputs[OUT_CH_TWO_OUTPUT].setVoltage(inputs[IN_CH_TWO_INPUT].getVoltage() * 2);
        outputs[OUT_CH_THR_OUTPUT].setVoltage(inputs[IN_CH_THR_INPUT].getVoltage() * 10);
        outputs[OUT_CH_FOU_OUTPUT].setVoltage(inputs[IN_CH_FOU_INPUT].getVoltage() * 12);
	}
};


struct AmpWidget : ModuleWidget {
	AmpWidget(Amp* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Amp.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.642, 11.577)), module, Amp::IN_CH_ONE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.642, 38.036)), module, Amp::IN_CH_TWO_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.642, 64.494)), module, Amp::IN_CH_THR_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.642, 90.952)), module, Amp::IN_CH_FOU_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.642, 22.16)), module, Amp::OUT_CH_ONE_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.642, 48.618)), module, Amp::OUT_CH_TWO_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.642, 75.076)), module, Amp::OUT_CH_THR_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.642, 101.535)), module, Amp::OUT_CH_FOU_OUTPUT));
	}
};


Model* modelAmp = createModel<Amp, AmpWidget>("MCXIVAmp");