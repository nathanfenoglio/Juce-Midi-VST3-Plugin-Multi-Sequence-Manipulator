/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MidiEffectAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Button::Listener
{
public:
    MidiEffectAudioProcessorEditor (MidiEffectAudioProcessor&);
    ~MidiEffectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    // SEQUENCE 1
    // functions/variables called/updated when input text is changed
    void input1TextChanged();
    std::vector <int> scaleFromInputText1;

    void modBy1Changed();
    int modByVal1;

    void addBy1Changed();
    int addByVal1;

    void octaves1Changed();
    int octaves1Val;

    void sumInvNum1Changed();
    int sumInvNum1Val;

    void tohNumDiscs1Changed();
    int tohNumDiscs1Val;

    void replaceThis1Changed();
    std::vector <int> replaceThis1;

    void replaceWith1Changed();
    std::vector <int> replaceWith1;

    // button pressed functions
    void modByAddBy1();
    void dontModByAddBy1();

    void addByModBy1();
    void dontAddByModBy1();

    void rotateLeft1();
    void rotateRight1();

    void reverseInput1();
    void dontReverseInput1();

    void invertInput1();
    void dontInvertInput1();

    void tohIt1();
    void dontTohIt1();

    void replaceThisWithThat1();
    void dontReplaceThisWithThat1();


    // SEQUENCE 2
    // functions/variables called/updated when input text is changed
    void input2TextChanged();
    std::vector <int> scaleFromInputText2;

    void modBy2Changed();
    int modByVal2;

    void addBy2Changed();
    int addByVal2;

    void octaves2Changed();
    int octaves2Val;

    void sumInvNum2Changed();
    int sumInvNum2Val;

    void tohNumDiscs2Changed();
    int tohNumDiscs2Val;

    void replaceThis2Changed();
    std::vector <int> replaceThis2;

    void replaceWith2Changed();
    std::vector <int> replaceWith2;

    // operations that involve both sequences
    void seq1ScalarChanged();
    int seq1ScalarVal;

    void seq2ScalarChanged();
    int seq2ScalarVal;

    void modByMultSeqsChanged();
    int modByMultSeqsVal;

    void octavesMultSeqsChanged();
    int octavesMultSeqsVal;

    // button pressed functions
    void modByAddBy2();
    void dontModByAddBy2();

    void addByModBy2();
    void dontAddByModBy2();

    void rotateLeft2();
    void rotateRight2();

    void reverseInput2();
    void dontReverseInput2();

    void invertInput2();
    void dontInvertInput2();

    void tohIt2();
    void dontTohIt2();

    void replaceThisWithThat2();
    void dontReplaceThisWithThat2();

    // functions that involve both sequences
    void spliceIt();
    void dontSpliceIt();

    void multiplySequencesByScalarsAddTogetherModBy();
    void dontMultiplySequencesByScalarsAddTogetherModBy();


    // handles all button clicks, checks are made for which button was pressed inside the function 
    void buttonClicked(juce::Button* button) override;

    // converts int vector to comma separated string
    juce::String convertVectToCommaStr(std::vector<int> nums);

    // parses a vector of ints from a comma separated string of ints
    std::vector<int> parseCommaSeparatedIntegers(const juce::String& str);

    // the last parameter true -> perform the addition 1st, then mod, false -> mod 1st, then perform the addition
    // numOctaves will add that amount of octaves to the output since modding by whatever value can make the output in a very low octave
    // can also use numOctaves just transpose a sequence however many octaves up or down 
    std::vector<int> modAllVectorByCertainAmountAndAddByCertainAmount(
                                    std::vector <int> theVector, int modBy, int addTo, 
                                    int numOctaves, bool addFirst);

    std::vector<int> multiple_vectors_to_one_vector_one_element_by_one_element(std::vector < std::vector <int> > all_vectors);

    std::vector<int> add_two_vectors_ability_to_mult_by_scalars_too(std::vector <int> vect_a, int scalar_a, std::vector <int> vect_b, int scalar_b, int mod_by, int num_octaves);
	
    class Tower_Of_Hanoi{
	public:
		int numDiscs;
		//represent with 2D list containing the order of the discs in each of the possible pole spaces
	    std::vector < std::vector <int> > discsOnPoles;
	    //just saving all of the states of the discs on poles in giant array to use
	    std::vector < std::vector < std::vector <int> > > discsOnPolesInTime;
		
		template <class T>
		T popAndReturn(std::vector <T> &vect);
		template <class T>
		T popFrontAndReturn(std::vector <T> &vect);
		std::vector <int> multipleVectorsToOneVectorOneElementByOneElement(std::vector < std::vector <int> > allVectorsOneTime, std::vector <int> scaleToTranslateTo);
		
		void move(int f, int t); //f - from, t - to
		void hanoi(int n, int f, int h, int t); //n - # of discs, f - from, h - helper pole (go between), t - to

	};

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    // you end up using it for ...
    MidiEffectAudioProcessor& audioProcessor;

    // SEQUENCE 1
    // input labels/boxes
    juce::Label inputText1Label;
    juce::Label inputText1;
    juce::Label input1MoveMenuLabel;
    juce::ComboBox input1MoveMenu;

    juce::Label modBy1Label;
    juce::Label modBy1Input;

    juce::Label addBy1Label;
    juce::Label addBy1Input;

    juce::Label octaves1Label;
    juce::Label octaves1Input;

    juce::Label sumInvNum1Label;
    juce::Label sumInvNum1Input;

    juce::Label tohNumDiscs1Label;
    juce::Label tohNumDiscs1Input;

    juce::Label replaceThis1Label;
    juce::Label replaceThis1Input;

    juce::Label replaceWith1Label;
    juce::Label replaceWith1Input;


    // SEQUENCE 2
    // input labels/boxes
    juce::Label inputText2Label;
    juce::Label inputText2;
    juce::Label input2MoveMenuLabel;
    juce::ComboBox input2MoveMenu;

    juce::Label modBy2Label;
    juce::Label modBy2Input;

    juce::Label addBy2Label;
    juce::Label addBy2Input;

    juce::Label octaves2Label;
    juce::Label octaves2Input;

    juce::Label sumInvNum2Label;
    juce::Label sumInvNum2Input;

    juce::Label tohNumDiscs2Label;
    juce::Label tohNumDiscs2Input;

    juce::Label replaceThis2Label;
    juce::Label replaceThis2Input;

    juce::Label replaceWith2Label;
    juce::Label replaceWith2Input;


    // OPERATIONS THAT INVOLVE BOTH SEQUENCES
    juce::Label multiplySequencesByScalarsAddTogetherModByLabel;
    
    juce::Label seq1ScalarLabel;
    juce::Label seq1ScalarInput;

    juce::Label seq2ScalarLabel;
    juce::Label seq2ScalarInput;

    juce::Label modByMultSeqsLabel;
    juce::Label modByMultSeqsInput;

    juce::Label octavesMultSeqsLabel;
    juce::Label octavesMultSeqsInput;

    // buttons
    // enum classes can be used for both sequences buttons
    enum class ModByAddByBtnState {
        ModByAddBy,
        NotModByAddBy
    };

    enum class AddByModByBtnState {
        AddByModBy,
        NotAddByModBy
    };

    enum class ReverseBtnState {
        Reverse,
        NotReverse
    };

    enum class SumInvBtnState {
        Invert,
        NotInvert
    };

    enum class TohItBtnState {
        TohIt,
        NotTohIt
    };

    enum class ApplyReplacementBtnState {
        Replace,
        NotReplace
    };

    enum class SpliceBtnState {
        Splice,
        NotSplice
    };

    enum class MultiplySequencesByScalarsAddTogetherModByBtnState {
        MultiplySequencesByScalarsAddTogetherModBy,
        NotMultiplySequencesByScalarsAddTogetherModBy
    };

    // SEQUENCE 1
    juce::TextButton modByAddBy1Btn;
    ModByAddByBtnState modByAddBy1State { ModByAddByBtnState::NotModByAddBy };

    juce::TextButton addByModBy1Btn;
    AddByModByBtnState addByModBy1State { AddByModByBtnState::NotAddByModBy };

    // rotate left and rotate right buttons
    // but these I want to change the original vector and display
    // not necessarily just go to the output display
    // not a toggle button, will just keep rotating every time the user presses
    juce::TextButton rotLeft1Btn;
    juce::TextButton rotRight1Btn;

    juce::TextButton reverse1Btn;
    ReverseBtnState reverse1State { ReverseBtnState::NotReverse };

    juce::TextButton sumInv1Btn;
    SumInvBtnState sumInv1State { SumInvBtnState::NotInvert };

    juce::TextButton tohIt1Btn;
    TohItBtnState tohIt1BtnState { TohItBtnState::NotTohIt };

    juce::TextButton applyReplacement1Btn;
    ApplyReplacementBtnState applyReplacement1BtnState { ApplyReplacementBtnState::NotReplace };


    // SEQUENCE 2
    juce::TextButton modByAddBy2Btn;
    ModByAddByBtnState modByAddBy2State { ModByAddByBtnState::NotModByAddBy };

    juce::TextButton addByModBy2Btn;
    AddByModByBtnState addByModBy2State { AddByModByBtnState::NotAddByModBy };

    // rotate left and rotate right buttons
    // but these I want to change the original vector and display
    // not necessarily just go to the output display
    // not a toggle button, will just keep rotating every time the user presses
    juce::TextButton rotLeft2Btn;
    juce::TextButton rotRight2Btn;

    juce::TextButton reverse2Btn;
    ReverseBtnState reverse2State { ReverseBtnState::NotReverse };

    juce::TextButton sumInv2Btn;
    SumInvBtnState sumInv2State { SumInvBtnState::NotInvert };

    juce::TextButton tohIt2Btn;
    TohItBtnState tohIt2BtnState { TohItBtnState::NotTohIt };

    juce::TextButton applyReplacement2Btn;
    ApplyReplacementBtnState applyReplacement2BtnState { ApplyReplacementBtnState::NotReplace };


    // buttons for operations that involve both sequences
    juce::Label spliceLabel;
    juce::TextButton spliceBtn;
    SpliceBtnState spliceBtnState { SpliceBtnState::NotSplice }; 

    juce::TextButton multiplySequencesByScalarsAddTogetherModByBtn;
    MultiplySequencesByScalarsAddTogetherModByBtnState multiplySequencesByScalarsAddTogetherModByBtnState { MultiplySequencesByScalarsAddTogetherModByBtnState::NotMultiplySequencesByScalarsAddTogetherModBy };


    // OUTPUT
    juce::Label outputText1Label;
    juce::Label outputText1;
    juce::Label output1MoveMenuLabel;
    juce::ComboBox output1MoveMenu;

    juce::Label outputText2Label;
    juce::Label outputText2;
    juce::Label output2MoveMenuLabel;
    juce::ComboBox output2MoveMenu;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiEffectAudioProcessorEditor)
};
