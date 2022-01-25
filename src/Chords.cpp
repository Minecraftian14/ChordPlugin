#include "plugin.hpp"


struct Chords : Module {

    enum MinorScale {
        FIRST = 0,
        SECOND = 2,
        THIRD = 3,
        FOURTH = 5,
        FIFTH = 7,
        SIXTH = 8,
        SEVENTH = 10
    };

//                                  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11};
    int mapFullNoteToSemiNote[12] = {0, 0, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6};
    int mapSemiNoteToFullNote[7] = {0, 2, 3, 5, 7, 8, 10};

    int sclArray[7] = {FIRST, SECOND, THIRD, FOURTH, FIFTH, SIXTH, SEVENTH};

    enum ParamId {
        PARAMS_LEN
    };
    enum InputId {
        IN_SOURCEOCTAVE_INPUT,
        IN_SOURCESCALE_INPUT,
        IN_SOURCECHORD_INPUT,
        INPUTS_LEN
    };
    enum OutputId {
        OUT_FREQUENCYONE_OUTPUT,
        OUT_FREQUENCYTWO_OUTPUT,
        OUT_FREQUENCYTHR_OUTPUT,
        OUTPUTS_LEN
    };
    enum LightId {
        LIGHTS_LEN
    };

    Chords() {
        config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
        configInput(IN_SOURCEOCTAVE_INPUT, "Octave");
        configInput(IN_SOURCESCALE_INPUT, "Scale");
        configInput(IN_SOURCECHORD_INPUT, "Chord");
        configOutput(OUT_FREQUENCYONE_OUTPUT, "First");
        configOutput(OUT_FREQUENCYTWO_OUTPUT, "Third");
        configOutput(OUT_FREQUENCYTHR_OUTPUT, "Fifth");
    }

    void process(const ProcessArgs &args) override {

        float pitch = inputs[IN_SOURCEOCTAVE_INPUT].getVoltage() + inputs[IN_SOURCESCALE_INPUT].getVoltage() / 12.f;
        pitch = clamp(pitch, -4.f, 4.f);
//        int scale = (int) (pitch * 12);

        int chord = flatCast(inputs[IN_SOURCECHORD_INPUT].getVoltage() * 12.f);
        while (chord < 12) chord += 12; // TODO: Was this required? NOTE: It's required IF % gives neg vals too
        chord %= 12;
        chord = mapFullNoteToSemiNote[chord];

        int first = mapSemiNoteToFullNote[chord];
        int secon = mapSemiNoteToFullNote[(chord + 2) % 7] + ((chord + 2) > 6 ? 12 : 0);
        int third = mapSemiNoteToFullNote[(chord + 4) % 7] + ((chord + 4) > 6 ? 12 : 0);

        outputs[OUT_FREQUENCYONE_OUTPUT].setVoltage(pitch + (float) first / 12.f);
        outputs[OUT_FREQUENCYTWO_OUTPUT].setVoltage(pitch + (float) secon / 12.f);
        outputs[OUT_FREQUENCYTHR_OUTPUT].setVoltage(pitch + (float) third / 12.f);
    }

    static int flatCast(float val) {
        int expansion = (int) (2.f * val);
        if (expansion % 2 == 0) return (int) val;
        return 1 + (int) val;
    }

};


struct ChordsWidget : ModuleWidget {
    ChordsWidget(Chords *module) {
        setModule(module);
        setPanel(createPanel(asset::plugin(pluginInstance, "res/Chords.svg")));

        addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
        addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
        addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
        addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.642, 11.048)), module, Chords::IN_SOURCEOCTAVE_INPUT));
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.642, 29.568)), module, Chords::IN_SOURCESCALE_INPUT));
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.642, 47.364)), module, Chords::IN_SOURCECHORD_INPUT));

        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.578, 68.779)), module, Chords::OUT_FREQUENCYONE_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.578, 87.299)), module, Chords::OUT_FREQUENCYTWO_OUTPUT));
        addOutput(
                createOutputCentered<PJ301MPort>(mm2px(Vec(7.578, 105.095)), module, Chords::OUT_FREQUENCYTHR_OUTPUT));
    }
};


Model *modelChords = createModel<Chords, ChordsWidget>("MCXIVChords");