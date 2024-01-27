/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <regex>

//==============================================================================
MidiEffectAudioProcessorEditor::MidiEffectAudioProcessorEditor (MidiEffectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    // setSize (600, 500);
    // setSize (800, 666);
    setSize (800, 800);
    
    // SEQUENCE 1
    addAndMakeVisible(inputText1Label); 
    inputText1Label.setText("Sequence 1", juce::dontSendNotification);
    inputText1Label.attachToComponent(&inputText1, false); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    inputText1Label.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(inputText1);
    inputText1.setEditable(true);
    inputText1.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);
    inputText1.onTextChange = [this] { input1TextChanged(); };

    // drop down menu option to move to another display box
    addAndMakeVisible(input1MoveMenuLabel);
    input1MoveMenuLabel.setText("Move To", juce::dontSendNotification);
    input1MoveMenuLabel.attachToComponent(&input1MoveMenu, false); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    input1MoveMenuLabel.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(input1MoveMenu);
    input1MoveMenu.setTextWhenNothingSelected("Move To");
    input1MoveMenu.addItem("Sequence 2", 2);
    input1MoveMenu.addItem("Output 1", 3);
    input1MoveMenu.addItem("Output 2", 4);
    input1MoveMenu.onChange = [this]() {
      if(input1MoveMenu.getSelectedItemIndex() == 0) { // Sequence 2
        inputText2.setText(inputText1.getText(), juce::dontSendNotification);
      }
      else if(input1MoveMenu.getSelectedItemIndex() == 1) { // Output 1
        outputText1.setText(inputText1.getText(), juce::dontSendNotification);
      }
      else if(input1MoveMenu.getSelectedItemIndex() == 2) { // Output 2
        outputText2.setText(inputText1.getText(), juce::dontSendNotification);
      }
    };


    addAndMakeVisible(modBy1Label);
    modBy1Label.setText("Mod By", juce::dontSendNotification);
    modBy1Label.attachToComponent(&modBy1Input, true); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    modBy1Label.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(modBy1Input);
    modBy1Input.setEditable(true);
    modBy1Input.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);
    modBy1Input.onTextChange = [this] { modBy1Changed(); };

    addAndMakeVisible(addBy1Label);
    addBy1Label.setText("Add By", juce::dontSendNotification);
    addBy1Label.attachToComponent(&addBy1Input, true); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    addBy1Label.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(addBy1Input);
    addBy1Input.setEditable(true);
    addBy1Input.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);
    addBy1Input.onTextChange = [this] { addBy1Changed(); };

    // # of octaves to add to note after mod by add by or add by mod by operations are performed
    addAndMakeVisible(octaves1Label);
    octaves1Label.setText("Octaves", juce::dontSendNotification);
    octaves1Label.attachToComponent(&octaves1Input, true); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    octaves1Label.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(octaves1Input);
    octaves1Input.setEditable(true);
    octaves1Input.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);
    octaves1Input.onTextChange = [this] { addBy1Changed(); };

    addAndMakeVisible(sumInvNum1Label);
    sumInvNum1Label.setText("Sum inversion #", juce::dontSendNotification);
    sumInvNum1Label.attachToComponent(&sumInvNum1Input, true); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    sumInvNum1Label.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(sumInvNum1Input);
    sumInvNum1Input.setEditable(true);
    sumInvNum1Input.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);
    sumInvNum1Input.onTextChange = [this] { sumInvNum1Changed(); };

    addAndMakeVisible(tohNumDiscs1Label);
    tohNumDiscs1Label.setText("# Discs TOH", juce::dontSendNotification);
    tohNumDiscs1Label.attachToComponent(&tohNumDiscs1Input, true); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    tohNumDiscs1Label.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(tohNumDiscs1Input);
    tohNumDiscs1Input.setEditable(true);
    tohNumDiscs1Input.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);
    tohNumDiscs1Input.onTextChange = [this] { tohNumDiscs1Changed(); };

    addAndMakeVisible(replaceThis1Label);
    replaceThis1Label.setText("Replace This", juce::dontSendNotification);
    replaceThis1Label.attachToComponent(&replaceThis1Input, true); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    replaceThis1Label.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(replaceThis1Input);
    replaceThis1Input.setEditable(true);
    replaceThis1Input.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);
    replaceThis1Input.onTextChange = [this] { replaceThis1Changed(); };

    addAndMakeVisible(replaceWith1Label);
    replaceWith1Label.setText("Replace With", juce::dontSendNotification);
    replaceWith1Label.attachToComponent(&replaceWith1Input, true); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    replaceWith1Label.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(replaceWith1Input);
    replaceWith1Input.setEditable(true);
    replaceWith1Input.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);
    replaceWith1Input.onTextChange = [this] { replaceWith1Changed(); };

    // buttons
    addAndMakeVisible(modByAddBy1Btn);
    // modByAddBy1Btn.setButtonText("Mod By Add By");
    modByAddBy1Btn.setButtonText("% | +");
    modByAddBy1Btn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::aquamarine);
    modByAddBy1Btn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    modByAddBy1Btn.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    modByAddBy1Btn.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
    modByAddBy1Btn.setToggleState(false, juce::dontSendNotification);
    modByAddBy1Btn.addListener(this);

    addAndMakeVisible(addByModBy1Btn);
    // addByModBy1Btn.setButtonText("Add By Mod By");
    addByModBy1Btn.setButtonText("+ | %");
    addByModBy1Btn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::aquamarine);
    addByModBy1Btn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    addByModBy1Btn.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    addByModBy1Btn.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
    addByModBy1Btn.setToggleState(false, juce::dontSendNotification);
    addByModBy1Btn.addListener(this);

    addAndMakeVisible(rotLeft1Btn);
    rotLeft1Btn.setButtonText("<-");
    // not a toggle button, will just keep rotating every time the user presses
    rotLeft1Btn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    rotLeft1Btn.setToggleState(false, juce::dontSendNotification);
    rotLeft1Btn.addListener(this);

    addAndMakeVisible(rotRight1Btn);
    rotRight1Btn.setButtonText("->");
    // not a toggle button, will just keep rotating every time the user presses
    rotRight1Btn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    rotRight1Btn.setToggleState(false, juce::dontSendNotification);
    rotRight1Btn.addListener(this);

    addAndMakeVisible(reverse1Btn);
    // reverse1Btn.setButtonText("Reverse");
    reverse1Btn.setButtonText("REV");
    reverse1Btn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::aquamarine);
    reverse1Btn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    reverse1Btn.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    reverse1Btn.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
    reverse1Btn.setToggleState(false, juce::dontSendNotification);
    reverse1Btn.addListener(this);

    addAndMakeVisible(sumInv1Btn);
    // sumInv1Btn.setButtonText("Invert");
    sumInv1Btn.setButtonText("INV");
    sumInv1Btn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::aquamarine);
    sumInv1Btn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    sumInv1Btn.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    sumInv1Btn.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
    sumInv1Btn.setToggleState(false, juce::dontSendNotification);
    sumInv1Btn.addListener(this);

    addAndMakeVisible(tohIt1Btn);
    tohIt1Btn.setButtonText("TOH");
    tohIt1Btn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::aquamarine);
    tohIt1Btn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    tohIt1Btn.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    tohIt1Btn.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
    tohIt1Btn.setToggleState(false, juce::dontSendNotification);
    tohIt1Btn.addListener(this);

    addAndMakeVisible(applyReplacement1Btn);
    applyReplacement1Btn.setButtonText("REPLACE");
    applyReplacement1Btn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::aquamarine);
    applyReplacement1Btn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    applyReplacement1Btn.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    applyReplacement1Btn.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
    applyReplacement1Btn.setToggleState(false, juce::dontSendNotification);
    applyReplacement1Btn.addListener(this);


    // SEQUENCE 2
    addAndMakeVisible(inputText2Label); 
    inputText2Label.setText("Sequence 2", juce::dontSendNotification);
    inputText2Label.attachToComponent(&inputText2, false); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    inputText2Label.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(inputText2);
    inputText2.setEditable(true);
    inputText2.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);
    inputText2.onTextChange = [this] { input2TextChanged(); };

    // drop down menu option to move to another display box
    addAndMakeVisible(input2MoveMenuLabel);
    input2MoveMenuLabel.setText("Move To", juce::dontSendNotification);
    input2MoveMenuLabel.attachToComponent(&input2MoveMenu, false); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    input2MoveMenuLabel.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(input2MoveMenu);
    input2MoveMenu.setTextWhenNothingSelected("Move To");
    input2MoveMenu.addItem("Sequence 1", 1);
    input2MoveMenu.addItem("Output 1", 3);
    input2MoveMenu.addItem("Output 2", 4);
    input2MoveMenu.onChange = [this]() {
      if(input2MoveMenu.getSelectedItemIndex() == 0) { // Sequence 1
        inputText1.setText(inputText2.getText(), juce::dontSendNotification);
      }
      else if(input2MoveMenu.getSelectedItemIndex() == 1) { // Output 1
        outputText1.setText(inputText2.getText(), juce::dontSendNotification);
      }
      else if(input2MoveMenu.getSelectedItemIndex() == 2) { // Output 2
        outputText2.setText(inputText2.getText(), juce::dontSendNotification);
      }
    };

    addAndMakeVisible(modBy2Label);
    modBy2Label.setText("Mod By", juce::dontSendNotification);
    modBy2Label.attachToComponent(&modBy2Input, true); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    modBy2Label.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(modBy2Input);
    modBy2Input.setEditable(true);
    modBy2Input.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);
    modBy2Input.onTextChange = [this] { modBy2Changed(); };

    addAndMakeVisible(addBy2Label);
    addBy2Label.setText("Add By", juce::dontSendNotification);
    addBy2Label.attachToComponent(&addBy2Input, true); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    addBy2Label.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(addBy2Input);
    addBy2Input.setEditable(true);
    addBy2Input.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);
    addBy2Input.onTextChange = [this] { addBy2Changed(); };

    // # of octaves to add to note after mod by add by or add by mod by operations are performed
    addAndMakeVisible(octaves2Label);
    octaves2Label.setText("Octaves", juce::dontSendNotification);
    octaves2Label.attachToComponent(&octaves2Input, true); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    octaves2Label.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(octaves2Input);
    octaves2Input.setEditable(true);
    octaves2Input.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);
    octaves2Input.onTextChange = [this] { addBy2Changed(); };

    addAndMakeVisible(sumInvNum2Label);
    sumInvNum2Label.setText("Sum inversion #", juce::dontSendNotification);
    sumInvNum2Label.attachToComponent(&sumInvNum2Input, true); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    sumInvNum2Label.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(sumInvNum2Input);
    sumInvNum2Input.setEditable(true);
    sumInvNum2Input.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);
    sumInvNum2Input.onTextChange = [this] { sumInvNum2Changed(); };

    addAndMakeVisible(tohNumDiscs2Label);
    tohNumDiscs2Label.setText("# Discs TOH", juce::dontSendNotification);
    tohNumDiscs2Label.attachToComponent(&tohNumDiscs2Input, true); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    tohNumDiscs2Label.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(tohNumDiscs2Input);
    tohNumDiscs2Input.setEditable(true);
    tohNumDiscs2Input.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);
    tohNumDiscs2Input.onTextChange = [this] { tohNumDiscs2Changed(); };

    addAndMakeVisible(replaceThis2Label);
    replaceThis2Label.setText("Replace This", juce::dontSendNotification);
    replaceThis2Label.attachToComponent(&replaceThis2Input, true); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    replaceThis2Label.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(replaceThis2Input);
    replaceThis2Input.setEditable(true);
    replaceThis2Input.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);
    replaceThis2Input.onTextChange = [this] { replaceThis2Changed(); };

    addAndMakeVisible(replaceWith2Label);
    replaceWith2Label.setText("Replace With", juce::dontSendNotification);
    replaceWith2Label.attachToComponent(&replaceWith2Input, true); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    replaceWith2Label.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(replaceWith2Input);
    replaceWith2Input.setEditable(true);
    replaceWith2Input.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);
    replaceWith2Input.onTextChange = [this] { replaceWith2Changed(); };


    // operations that involve both sequences
    addAndMakeVisible(multiplySequencesByScalarsAddTogetherModByLabel);
    multiplySequencesByScalarsAddTogetherModByLabel.setText("Multiply Sequences By Scalars, Add Together, Mod By, Add Octaves", juce::dontSendNotification);
    multiplySequencesByScalarsAddTogetherModByLabel.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(seq1ScalarLabel);
    seq1ScalarLabel.setText("Seq 1 *", juce::dontSendNotification);
    seq1ScalarLabel.attachToComponent(&seq1ScalarInput, true); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    seq1ScalarLabel.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(seq1ScalarInput);
    seq1ScalarInput.setEditable(true);
    seq1ScalarInput.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);
    seq1ScalarInput.onTextChange = [this] { seq1ScalarChanged(); };

    addAndMakeVisible(seq2ScalarLabel);
    seq2ScalarLabel.setText("Seq 2 *", juce::dontSendNotification);
    seq2ScalarLabel.attachToComponent(&seq2ScalarInput, true); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    seq2ScalarLabel.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(seq2ScalarInput);
    seq2ScalarInput.setEditable(true);
    seq2ScalarInput.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);
    seq2ScalarInput.onTextChange = [this] { seq2ScalarChanged(); };

    addAndMakeVisible(modByMultSeqsLabel);
    modByMultSeqsLabel.setText("Mod By", juce::dontSendNotification);
    modByMultSeqsLabel.attachToComponent(&modByMultSeqsInput, true); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    modByMultSeqsLabel.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(modByMultSeqsInput);
    modByMultSeqsInput.setEditable(true);
    modByMultSeqsInput.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);
    modByMultSeqsInput.onTextChange = [this] { modByMultSeqsChanged(); };

    addAndMakeVisible(octavesMultSeqsLabel);
    octavesMultSeqsLabel.setText("Octaves", juce::dontSendNotification);
    octavesMultSeqsLabel.attachToComponent(&octavesMultSeqsInput, true); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    octavesMultSeqsLabel.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(octavesMultSeqsInput);
    octavesMultSeqsInput.setEditable(true);
    octavesMultSeqsInput.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);
    octavesMultSeqsInput.onTextChange = [this] { octavesMultSeqsChanged(); };


    // buttons
    addAndMakeVisible(modByAddBy2Btn);
    // modByAddBy2Btn.setButtonText("Mod By Add By");
    modByAddBy2Btn.setButtonText("% | +");
    modByAddBy2Btn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::aquamarine);
    modByAddBy2Btn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    modByAddBy2Btn.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    modByAddBy2Btn.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
    modByAddBy2Btn.setToggleState(false, juce::dontSendNotification);
    modByAddBy2Btn.addListener(this);

    addAndMakeVisible(addByModBy2Btn);
    // addByModBy2Btn.setButtonText("Add By Mod By");
    addByModBy2Btn.setButtonText("+ | %");
    addByModBy2Btn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::aquamarine);
    addByModBy2Btn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    addByModBy2Btn.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    addByModBy2Btn.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
    addByModBy2Btn.setToggleState(false, juce::dontSendNotification);
    addByModBy2Btn.addListener(this);

    addAndMakeVisible(rotLeft2Btn);
    rotLeft2Btn.setButtonText("<-");
    // not a toggle button, will just keep rotating every time the user presses
    rotLeft2Btn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    rotLeft2Btn.setToggleState(false, juce::dontSendNotification);
    rotLeft2Btn.addListener(this);

    addAndMakeVisible(rotRight2Btn);
    rotRight2Btn.setButtonText("->");
    // not a toggle button, will just keep rotating every time the user presses
    rotRight2Btn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    rotRight2Btn.setToggleState(false, juce::dontSendNotification);
    rotRight2Btn.addListener(this);

    addAndMakeVisible(reverse2Btn);
    // reverse2Btn.setButtonText("Reverse");
    reverse2Btn.setButtonText("REV");
    reverse2Btn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::aquamarine);
    reverse2Btn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    reverse2Btn.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    reverse2Btn.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
    reverse2Btn.setToggleState(false, juce::dontSendNotification);
    reverse2Btn.addListener(this);

    addAndMakeVisible(sumInv2Btn);
    // sumInv2Btn.setButtonText("Invert");
    sumInv2Btn.setButtonText("INV");
    sumInv2Btn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::aquamarine);
    sumInv2Btn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    sumInv2Btn.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    sumInv2Btn.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
    sumInv2Btn.setToggleState(false, juce::dontSendNotification);
    sumInv2Btn.addListener(this);

    addAndMakeVisible(tohIt2Btn);
    tohIt2Btn.setButtonText("TOH");
    tohIt2Btn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::aquamarine);
    tohIt2Btn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    tohIt2Btn.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    tohIt2Btn.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
    tohIt2Btn.setToggleState(false, juce::dontSendNotification);
    tohIt2Btn.addListener(this);

    addAndMakeVisible(applyReplacement2Btn);
    applyReplacement2Btn.setButtonText("REPLACE");
    applyReplacement2Btn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::aquamarine);
    applyReplacement2Btn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    applyReplacement2Btn.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    applyReplacement2Btn.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
    applyReplacement2Btn.setToggleState(false, juce::dontSendNotification);
    applyReplacement2Btn.addListener(this);


    // OPERATIONS THAT INVOLVE BOTH SEQUENCES
    addAndMakeVisible(spliceLabel);
    spliceLabel.setText("Splice Seq 1 & Seq 2 Together", juce::dontSendNotification);
    spliceLabel.attachToComponent(&spliceBtn, true); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    spliceLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    
    addAndMakeVisible(spliceBtn);
    spliceBtn.setButtonText("SPLICE");
    spliceBtn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::aquamarine);
    spliceBtn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    spliceBtn.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    spliceBtn.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
    spliceBtn.setToggleState(false, juce::dontSendNotification);
    spliceBtn.addListener(this);

    addAndMakeVisible(multiplySequencesByScalarsAddTogetherModByBtn);
    multiplySequencesByScalarsAddTogetherModByBtn.setButtonText("* SEQS BY SCALARS + TOGETHER %");
    multiplySequencesByScalarsAddTogetherModByBtn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::aquamarine);
    multiplySequencesByScalarsAddTogetherModByBtn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    multiplySequencesByScalarsAddTogetherModByBtn.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    multiplySequencesByScalarsAddTogetherModByBtn.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
    multiplySequencesByScalarsAddTogetherModByBtn.setToggleState(false, juce::dontSendNotification);
    multiplySequencesByScalarsAddTogetherModByBtn.addListener(this);


    // OUTPUT
    addAndMakeVisible(outputText1Label); 
    outputText1Label.setText("Output 1", juce::dontSendNotification);
    outputText1Label.attachToComponent(&outputText1, false); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    outputText1Label.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(outputText1);
    outputText1.setEditable(true);
    outputText1.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);

    // drop down menu option to move to another display box
    addAndMakeVisible(output1MoveMenuLabel);
    output1MoveMenuLabel.setText("Move To", juce::dontSendNotification);
    output1MoveMenuLabel.attachToComponent(&output1MoveMenu, false); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    output1MoveMenuLabel.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(output1MoveMenu);
    output1MoveMenu.setTextWhenNothingSelected("Move To");
    output1MoveMenu.addItem("Sequence 1", 1);
    output1MoveMenu.addItem("Sequence 2", 2);
    output1MoveMenu.addItem("Output 2", 4);
    output1MoveMenu.onChange = [this]() {
      if(output1MoveMenu.getSelectedItemIndex() == 0) { // Sequence 1
        inputText1.setText(outputText1.getText(), juce::dontSendNotification);
      }
      else if(output1MoveMenu.getSelectedItemIndex() == 1) { // Sequence 2
        inputText2.setText(outputText1.getText(), juce::dontSendNotification);
      }
      else if(output1MoveMenu.getSelectedItemIndex() == 2) { // Output 2
        outputText2.setText(outputText1.getText(), juce::dontSendNotification);
      }
    };

    addAndMakeVisible(outputText2Label); 
    outputText2Label.setText("Output 2", juce::dontSendNotification);
    outputText2Label.attachToComponent(&outputText2, false); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    outputText2Label.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(outputText2);
    outputText2.setEditable(true);
    outputText2.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);

    // drop down menu option to move to another display box
    addAndMakeVisible(output2MoveMenuLabel);
    output2MoveMenuLabel.setText("Move To", juce::dontSendNotification);
    output2MoveMenuLabel.attachToComponent(&output2MoveMenu, false); // onLeft	if true, the label will stay on the left of its component; if false, it will stay above it
    output2MoveMenuLabel.setColour(juce::Label::textColourId, juce::Colours::orange);

    addAndMakeVisible(output2MoveMenu);
    output2MoveMenu.setTextWhenNothingSelected("Move To");
    output2MoveMenu.addItem("Sequence 1", 1);
    output2MoveMenu.addItem("Sequence 2", 2);
    output2MoveMenu.addItem("Output 1", 3);
    output2MoveMenu.onChange = [this]() {
      if(output2MoveMenu.getSelectedItemIndex() == 0) { // Sequence 1
        inputText1.setText(outputText2.getText(), juce::dontSendNotification);
      }
      else if(output2MoveMenu.getSelectedItemIndex() == 1) { // Sequence 2
        inputText2.setText(outputText2.getText(), juce::dontSendNotification);
      }
      else if(output2MoveMenu.getSelectedItemIndex() == 2) { // Output 2
        outputText1.setText(outputText2.getText(), juce::dontSendNotification);
      }
    };

}

MidiEffectAudioProcessorEditor::~MidiEffectAudioProcessorEditor()
{
}

//==============================================================================
void MidiEffectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);

}

void MidiEffectAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto btnType1_width = getWidth() * 0.08;
    auto btnType1_height = getWidth() * 0.04;

    // SEQUENCE 1
    inputText1.setBounds(50, 50, getWidth() - 110 - 60, 25);
    // adding drop down menus for option to move output to whatever display box
    input1MoveMenu.setBounds(680, 50, btnType1_width + 60, btnType1_height - 10);

    int startXOperationLine1 = 100;
    int startYOperationLine1 = 85;
    int heightOperationLine1 = 25;
    int spaceXOperationLine1 = 10;
    modBy1Input.setBounds(startXOperationLine1, startYOperationLine1, 40, heightOperationLine1);
    addBy1Input.setBounds(startXOperationLine1 + 100 + spaceXOperationLine1, startYOperationLine1, 40, heightOperationLine1);
    octaves1Input.setBounds(startXOperationLine1 + 200 + (spaceXOperationLine1 * 2), startYOperationLine1, 40, heightOperationLine1);
    sumInvNum1Input.setBounds(startXOperationLine1 + 355 + (spaceXOperationLine1 * 3), startYOperationLine1, 40, heightOperationLine1);
    tohNumDiscs1Input.setBounds(startXOperationLine1 + 480 + (spaceXOperationLine1 * 4), startYOperationLine1, 40, heightOperationLine1);

    int startXOperationLine2 = 135;
    int startYOperationLine2 = 165;
    int heightOperationLine2 = 25;
    int widthOperationLine2 = 225;
    int spaceXOperationLine2 = 225;
    replaceThis1Input.setBounds(startXOperationLine2, startYOperationLine2, widthOperationLine2, heightOperationLine2);
    replaceWith1Input.setBounds(startXOperationLine2 + 100 + spaceXOperationLine2, startYOperationLine2, widthOperationLine2, heightOperationLine2);

    int startX1 = 50;
    int spaceX1 = 25;
    int startY1 = 125;
    modByAddBy1Btn.setBounds(startX1, startY1, btnType1_width, btnType1_height);
    addByModBy1Btn.setBounds(startX1 + btnType1_width + spaceX1, startY1, btnType1_width, btnType1_height);
    rotLeft1Btn.setBounds(startX1 + (btnType1_width * 2) + (spaceX1 * 2), startY1, btnType1_width, btnType1_height);
    rotRight1Btn.setBounds(startX1 + (btnType1_width * 3) + (spaceX1 * 3), startY1, btnType1_width, btnType1_height);
    reverse1Btn.setBounds(startX1 + (btnType1_width * 4) + (spaceX1 * 4), startY1, btnType1_width, btnType1_height);
    sumInv1Btn.setBounds(startX1 + (btnType1_width * 5) + (spaceX1 * 5), startY1, btnType1_width, btnType1_height);
    tohIt1Btn.setBounds(startX1 + (btnType1_width * 6) + (spaceX1 * 6), startY1, btnType1_width, btnType1_height);

    // int startX2 = 325;
    // int spaceX2 = 25;
    // int startY2 = 200;
    // applyReplacement1Btn.setBounds(startX2, startY2, btnType1_width, btnType1_height);
    // moving replace button to side instead of under replace this and replace with fields
    applyReplacement1Btn.setBounds(700, startYOperationLine2 - 5, btnType1_width, btnType1_height);


    // SEQUENCE 2
    inputText2.setBounds(50, 300, getWidth() - 110 - 60, 25);
    // adding drop down menus for option to move output to whatever display box
    input2MoveMenu.setBounds(680, 300, btnType1_width + 60, btnType1_height - 10);

    int startXOperationLine3 = 100;
    int startYOperationLine3 = 335;
    int heightOperationLine3 = 25;
    int spaceXOperationLine3 = 10;
    modBy2Input.setBounds(startXOperationLine3, startYOperationLine3, 40, heightOperationLine3);
    addBy2Input.setBounds(startXOperationLine3 + 100 + spaceXOperationLine3, startYOperationLine3, 40, heightOperationLine3);
    octaves2Input.setBounds(startXOperationLine3 + 200 + (spaceXOperationLine3 * 2), startYOperationLine3, 40, heightOperationLine3);
    sumInvNum2Input.setBounds(startXOperationLine3 + 355 + (spaceXOperationLine3 * 3), startYOperationLine3, 40, heightOperationLine3);
    tohNumDiscs2Input.setBounds(startXOperationLine3 + 480 + (spaceXOperationLine3 * 4), startYOperationLine3, 40, heightOperationLine3);

    int startXOperationLine4 = 135;
    int startYOperationLine4 = 415;
    int heightOperationLine4 = 25;
    int widthOperationLine4 = 225;
    int spaceXOperationLine4 = 225;
    replaceThis2Input.setBounds(startXOperationLine4, startYOperationLine4, widthOperationLine4, heightOperationLine4);
    replaceWith2Input.setBounds(startXOperationLine4 + 100 + spaceXOperationLine4, startYOperationLine4, widthOperationLine4, heightOperationLine4);

    int startX3 = 50;
    int spaceX3 = 25;
    int startY3 = 375;
    modByAddBy2Btn.setBounds(startX3, startY3, btnType1_width, btnType1_height);
    addByModBy2Btn.setBounds(startX3 + btnType1_width + spaceX3, startY3, btnType1_width, btnType1_height);
    rotLeft2Btn.setBounds(startX3 + (btnType1_width * 2) + (spaceX3 * 2), startY3, btnType1_width, btnType1_height);
    rotRight2Btn.setBounds(startX3 + (btnType1_width * 3) + (spaceX3 * 3), startY3, btnType1_width, btnType1_height);
    reverse2Btn.setBounds(startX3 + (btnType1_width * 4) + (spaceX3 * 4), startY3, btnType1_width, btnType1_height);
    sumInv2Btn.setBounds(startX3 + (btnType1_width * 5) + (spaceX3 * 5), startY3, btnType1_width, btnType1_height);
    tohIt2Btn.setBounds(startX3 + (btnType1_width * 6) + (spaceX3 * 6), startY3, btnType1_width, btnType1_height);

    applyReplacement2Btn.setBounds(700, startYOperationLine4 - 5, btnType1_width, btnType1_height);

    // OPERATIONS THAT INVOLVE BOTH SEQUENCES
    // spliceBtn.setBounds(325, 500, btnType1_width, btnType1_height);
    spliceBtn.setBounds(250, 500, btnType1_width, btnType1_height);

    multiplySequencesByScalarsAddTogetherModByLabel.setBounds(50, 550, getWidth() - 110, 20);
    int startXOperationLine5 = 100;
    int startYOperationLine5 = 600;
    int heightOperationLine5 = 25;
    int spaceXOperationLine5 = 10;
    seq1ScalarInput.setBounds(startXOperationLine5, startYOperationLine5, 40, heightOperationLine5);
    seq2ScalarInput.setBounds(startXOperationLine5 + 100 + spaceXOperationLine5, startYOperationLine5, 40, heightOperationLine5);
    modByMultSeqsInput.setBounds(startXOperationLine5 + 200 + (spaceXOperationLine5 * 2), startYOperationLine5, 40, heightOperationLine5);
    octavesMultSeqsInput.setBounds(startXOperationLine5 + 300 + (spaceXOperationLine5 * 3), startYOperationLine5, 40, heightOperationLine5);

    // multiplySequencesByScalarsAddTogetherModByBtn.setBounds(325, 650, btnType1_width, btnType1_height);
    multiplySequencesByScalarsAddTogetherModByBtn.setBounds(startXOperationLine5 + 350 + (spaceXOperationLine5 * 4), startYOperationLine5 - 2, btnType1_width + 175, btnType1_height);

    
    // OUTPUT    
    // adding drop down menus for option to move output to whatever display box
    outputText1.setBounds(50, 700, getWidth() - 110 - 60, 20);
    output1MoveMenu.setBounds(680, 697, btnType1_width + 60, btnType1_height - 10);
    
    outputText2.setBounds(50, 750, getWidth() - 110 - 60, 20);
    output2MoveMenu.setBounds(680, 747, btnType1_width + 60, btnType1_height - 10);

}

// SEQUENCE 1
void MidiEffectAudioProcessorEditor::input1TextChanged() {
    scaleFromInputText1 = parseCommaSeparatedIntegers(inputText1.getText());
    audioProcessor._midiNotes = scaleFromInputText1;
}

void MidiEffectAudioProcessorEditor::modBy1Changed() {
    juce::String modByVal1Str = modBy1Input.getText();
    modByVal1 = std::stoi(modByVal1Str.toStdString());
}

void MidiEffectAudioProcessorEditor::addBy1Changed() {
    juce::String addByVal1Str = addBy1Input.getText();
    addByVal1 = std::stoi(addByVal1Str.toStdString());
}

void MidiEffectAudioProcessorEditor::octaves1Changed() {
    juce::String octaves1ValStr = octaves1Input.getText();
    octaves1Val = std::stoi(octaves1ValStr.toStdString());
}

void MidiEffectAudioProcessorEditor::sumInvNum1Changed() {
    juce::String sumInvNum1InputString = sumInvNum1Input.getText();
    sumInvNum1Val = std::stoi(sumInvNum1InputString.toStdString());
}

void MidiEffectAudioProcessorEditor::tohNumDiscs1Changed() {
    juce::String tohNumDiscs1InputString = tohNumDiscs1Input.getText();
    tohNumDiscs1Val = std::stoi(tohNumDiscs1InputString.toStdString());
}

void MidiEffectAudioProcessorEditor::replaceThis1Changed() {
    replaceThis1 = parseCommaSeparatedIntegers(replaceThis1Input.getText());
}

void MidiEffectAudioProcessorEditor::replaceWith1Changed() {
    replaceWith1 = parseCommaSeparatedIntegers(replaceWith1Input.getText());
}

// button pressed functions
void MidiEffectAudioProcessorEditor::modByAddBy1() {
    modByAddBy1State = ModByAddByBtnState::ModByAddBy;
    modByAddBy1Btn.setToggleState(true, juce::dontSendNotification);
  
    scaleFromInputText1 = parseCommaSeparatedIntegers(inputText1.getText());

    if(modByVal1 == 0) {
      return;
    }

    juce::String octaves1ValStr = octaves1Input.getText();
    // try catch to check if octave 1 input is not valid, if not set to 0
    try {
        octaves1Val = std::stoi(octaves1ValStr.toStdString());
    } 
    catch (const std::invalid_argument& e) {
        // Conversion failed, set the value to 0
        octaves1Val = 0;
    } 
    catch (const std::out_of_range& e) {
        // The converted value is out of the range of representable values, set to 0
        octaves1Val = 0;
    }

		std::vector <int> modified_vector = modAllVectorByCertainAmountAndAddByCertainAmount(scaleFromInputText1, modByVal1, addByVal1, octaves1Val, false);

    audioProcessor._midiNotes = modified_vector;

    // set up output as string with commas
    juce::String moddedByAddedByText = convertVectToCommaStr(modified_vector);
    outputText1.setText(moddedByAddedByText, juce::dontSendNotification);
}

void MidiEffectAudioProcessorEditor::dontModByAddBy1() {
    modByAddBy1State = ModByAddByBtnState::NotModByAddBy;
    modByAddBy1Btn.setToggleState(false, juce::dontSendNotification);

    std::vector<int> scaleFromInputText1 = parseCommaSeparatedIntegers(inputText1.getText());
    audioProcessor._midiNotes = scaleFromInputText1;

    outputText1.setText("", juce::dontSendNotification);

}

void MidiEffectAudioProcessorEditor::addByModBy1() {
    addByModBy1State = AddByModByBtnState::AddByModBy;
    addByModBy1Btn.setToggleState(true, juce::dontSendNotification);

    scaleFromInputText1 = parseCommaSeparatedIntegers(inputText1.getText());

    if(modByVal1 == 0) {
      return;
    }

    juce::String octaves1ValStr = octaves1Input.getText();
    // octaves1Val = std::stoi(octaves1ValStr.toStdString());
    // try catch to check if octave 1 input is not valid, if not set to 0
    try {
        octaves1Val = std::stoi(octaves1ValStr.toStdString());
    } 
    catch (const std::invalid_argument& e) {
        // Conversion failed, set the value to 0
        octaves1Val = 0;
    } 
    catch (const std::out_of_range& e) {
        // The converted value is out of the range of representable values, set to 0
        octaves1Val = 0;
    }
    
		std::vector <int> modified_vector = modAllVectorByCertainAmountAndAddByCertainAmount(scaleFromInputText1, modByVal1, addByVal1, octaves1Val, true);

    audioProcessor._midiNotes = modified_vector;

    // set up output as string with commas
    juce::String addedByModdedByText = convertVectToCommaStr(modified_vector);
    outputText1.setText(addedByModdedByText, juce::dontSendNotification);

}

void MidiEffectAudioProcessorEditor::dontAddByModBy1() {
    addByModBy1State = AddByModByBtnState::NotAddByModBy;
    addByModBy1Btn.setToggleState(false, juce::dontSendNotification);

    std::vector<int> scaleFromInputText1 = parseCommaSeparatedIntegers(inputText1.getText());
    audioProcessor._midiNotes = scaleFromInputText1;

    outputText1.setText("", juce::dontSendNotification);

}

void MidiEffectAudioProcessorEditor::rotateLeft1() {
    std::vector<int> scaleFromInputText1 = parseCommaSeparatedIntegers(inputText1.getText());

    std::rotate(scaleFromInputText1.begin(), scaleFromInputText1.begin() + 1, scaleFromInputText1.end());
    
    audioProcessor._midiNotes = scaleFromInputText1;

    juce::String rotLeftText = convertVectToCommaStr(scaleFromInputText1);
    inputText1.setText(rotLeftText, juce::dontSendNotification);

}

void MidiEffectAudioProcessorEditor::rotateRight1() {
    std::vector<int> scaleFromInputText1 = parseCommaSeparatedIntegers(inputText1.getText());

    // rotate right using reverse iterators
    std::rotate(scaleFromInputText1.rbegin(), scaleFromInputText1.rbegin() + 1, scaleFromInputText1.rend());
    
    audioProcessor._midiNotes = scaleFromInputText1;

    juce::String rotRightText = convertVectToCommaStr(scaleFromInputText1);
    inputText1.setText(rotRightText, juce::dontSendNotification);

}

void MidiEffectAudioProcessorEditor::reverseInput1() {
    reverse1State = ReverseBtnState::Reverse;
    reverse1Btn.setToggleState(true, juce::dontSendNotification);

    scaleFromInputText1 = parseCommaSeparatedIntegers(inputText1.getText());

		std::vector <int> copy;
		for(int i = 0; i < scaleFromInputText1.size(); i++){
			copy.push_back(scaleFromInputText1[i]);
		}
		// reverse
		for(int i = 0; i < scaleFromInputText1.size(); i++){
			scaleFromInputText1[i] = copy[scaleFromInputText1.size() - 1 - i];
		}

    audioProcessor._midiNotes = scaleFromInputText1;

    // set up output as string with commas
    juce::String reversedText = convertVectToCommaStr(scaleFromInputText1);
    outputText1.setText(reversedText, juce::dontSendNotification);
}

void MidiEffectAudioProcessorEditor::dontReverseInput1() {
    reverse1State = ReverseBtnState::NotReverse;
    reverse1Btn.setToggleState(false, juce::dontSendNotification);

    std::vector<int> scaleFromInputText1 = parseCommaSeparatedIntegers(inputText1.getText());
    audioProcessor._midiNotes = scaleFromInputText1;

    outputText1.setText("", juce::dontSendNotification);
}

void MidiEffectAudioProcessorEditor::invertInput1() {
    sumInv1State = SumInvBtnState::Invert;
    sumInv1Btn.setToggleState(true, juce::dontSendNotification);

    scaleFromInputText1 = parseCommaSeparatedIntegers(inputText1.getText());

    std::vector <int> changed_vector;
    int octave = 12;
    //so you want to take it down to a # in between 0-11
    //then do the sum inversion thing
    //then add back on the # of octaves that you reduced the original by
    for(int i = 0; i < scaleFromInputText1.size(); i++){
        int cur_num = scaleFromInputText1[i];
        int numOctaves = 0;
        while(cur_num >= 0){
            cur_num = cur_num - octave;
            numOctaves++;
        }
        //add 12 back and decrease # of octaves once since went one more before the while loop condition was violated
        cur_num = cur_num + octave;        
        numOctaves--;
        int inverted_num = ((sumInvNum1Val + octave) - cur_num) % octave;
        inverted_num = inverted_num + (numOctaves * octave);
        changed_vector.push_back(inverted_num);
    }

    audioProcessor._midiNotes = changed_vector;

    // set up output as string with commas
    juce::String invertedText = convertVectToCommaStr(changed_vector);
    outputText1.setText(invertedText, juce::dontSendNotification);

}

void MidiEffectAudioProcessorEditor::dontInvertInput1() {
    sumInv1State = SumInvBtnState::NotInvert;
    sumInv1Btn.setToggleState(false, juce::dontSendNotification);

    std::vector<int> scaleFromInputText1 = parseCommaSeparatedIntegers(inputText1.getText());
    audioProcessor._midiNotes = scaleFromInputText1;

    outputText1.setText("", juce::dontSendNotification);

}

void MidiEffectAudioProcessorEditor::tohIt1() {
    // ok there's like an entire class of Tower of Hanoi stuff that you need to implement and stuff...
    tohIt1BtnState = TohItBtnState::TohIt;
    tohIt1Btn.setToggleState(true, juce::dontSendNotification);

    std::vector<int> scaleFromInputText1 = parseCommaSeparatedIntegers(inputText1.getText());
    Tower_Of_Hanoi toh;
    toh.numDiscs = tohNumDiscs1Val;

    //set up original state with all discs in order on the leftest pole
		std::vector <int> start_pole;
		for(int i = 0; i < tohNumDiscs1Val; i++){
			start_pole.push_back(i + 1); //i + 1 because numbering the discs 1 through numDiscs
		}
		toh.discsOnPoles.push_back(start_pole);
		std::vector <int> empty_vector_1;
		std::vector <int> empty_vector_2;
		toh.discsOnPoles.push_back(empty_vector_1);
		toh.discsOnPoles.push_back(empty_vector_2);
		//just saving all of the states of the discs on poles in giant array to use
		toh.discsOnPolesInTime.push_back(toh.discsOnPoles);
			
		//hanoi(number of discs, start pole, other pole, goal pole) 
		toh.hanoi(tohNumDiscs1Val, 0, 1, 2);

		//interleave shuffle one by one element whatever you want to call it for all of the poles for all of the times
		std::vector <int> all_shuffled_throughout_all_of_time;
		for(int i = 0; i < toh.discsOnPolesInTime.size(); i++){
			std::vector <int> all_discs_interleaved_one_point_in_time = toh.multipleVectorsToOneVectorOneElementByOneElement(toh.discsOnPolesInTime[i], scaleFromInputText1); 
			for(int j = 0; j < all_discs_interleaved_one_point_in_time.size(); j++){
				all_shuffled_throughout_all_of_time.push_back(all_discs_interleaved_one_point_in_time[j]);
			}
		}

    audioProcessor._midiNotes = all_shuffled_throughout_all_of_time;

    // set up output as string with commas
    juce::String tohdText = convertVectToCommaStr(all_shuffled_throughout_all_of_time);
    outputText1.setText(tohdText, juce::dontSendNotification);
}

void MidiEffectAudioProcessorEditor::dontTohIt1 () {
    tohIt1BtnState = TohItBtnState::NotTohIt;
    tohIt1Btn.setToggleState(false, juce::dontSendNotification);

    // CRASHING HERE, THINKING THAT BECAUSE THE BUFFER IS TOO FULL WITH THE BIG VECTOR
    // YEA IF YOU STOP PLAYING AND THEN TOGGLE IT OFF, NO CRASH
    // SO NEED TO FIGURE OUT HOW TO STOP/EMPTY BUFFER ETC
    // BEFORE ANY CHANGE WHILE PLAY IS ON IN DAW...
    std::vector<int> scaleFromInputText1 = parseCommaSeparatedIntegers(inputText1.getText());
    audioProcessor._midiNotes = scaleFromInputText1;

    outputText1.setText("", juce::dontSendNotification);

}

void MidiEffectAudioProcessorEditor::replaceThisWithThat1() {
    applyReplacement1BtnState = ApplyReplacementBtnState::Replace;
    applyReplacement1Btn.setToggleState(true, juce::dontSendNotification);

    // std::vector<int> scaleFromInputText1 = parseCommaSeparatedIntegers(inputText1.getText());
    std::regex reg(convertVectToCommaStr(replaceThis1).toStdString()); //convert int replaceThis1 to string for regex
    std::string replacementStr = replaceWith1Input.getText().toStdString();
		std::string modifiedStr = std::regex_replace(inputText1.getText().toStdString(), reg, replacementStr);
    // juce::String juceModifiedStr = juce::String::fromStdString(modifiedStr);
    juce::String juceModifiedStr = modifiedStr;
    audioProcessor._midiNotes = parseCommaSeparatedIntegers(juceModifiedStr);

    outputText1.setText(juceModifiedStr, juce::dontSendNotification);
}

void MidiEffectAudioProcessorEditor::dontReplaceThisWithThat1() {
    applyReplacement1BtnState = ApplyReplacementBtnState::NotReplace;
    // ...
    applyReplacement1Btn.setToggleState(false, juce::dontSendNotification);

    std::vector<int> scaleFromInputText1 = parseCommaSeparatedIntegers(inputText1.getText());
    audioProcessor._midiNotes = scaleFromInputText1;

    outputText1.setText("", juce::dontSendNotification);

}


// SEQUENCE 2
void MidiEffectAudioProcessorEditor::input2TextChanged() {
    scaleFromInputText2 = parseCommaSeparatedIntegers(inputText2.getText());
    audioProcessor._midiNotes = scaleFromInputText2;
}

void MidiEffectAudioProcessorEditor::modBy2Changed() {
    juce::String modByVal2Str = modBy2Input.getText();
    modByVal2 = std::stoi(modByVal2Str.toStdString());
}

void MidiEffectAudioProcessorEditor::addBy2Changed() {
    juce::String addByVal2Str = addBy2Input.getText();
    addByVal2 = std::stoi(addByVal2Str.toStdString());
}

void MidiEffectAudioProcessorEditor::octaves2Changed() {
    juce::String octaves2ValStr = octaves2Input.getText();
    octaves2Val = std::stoi(octaves2ValStr.toStdString());
}

void MidiEffectAudioProcessorEditor::sumInvNum2Changed() {
    juce::String sumInvNum2InputString = sumInvNum2Input.getText();
    sumInvNum2Val = std::stoi(sumInvNum2InputString.toStdString());
}

void MidiEffectAudioProcessorEditor::tohNumDiscs2Changed() {
    juce::String tohNumDiscs2InputString = tohNumDiscs2Input.getText();
    tohNumDiscs2Val = std::stoi(tohNumDiscs2InputString.toStdString());
}

void MidiEffectAudioProcessorEditor::replaceThis2Changed() {
    replaceThis2 = parseCommaSeparatedIntegers(replaceThis2Input.getText());
}

void MidiEffectAudioProcessorEditor::replaceWith2Changed() {
    replaceWith2 = parseCommaSeparatedIntegers(replaceWith2Input.getText());
}

// button pressed functions
void MidiEffectAudioProcessorEditor::modByAddBy2() {
    modByAddBy2State = ModByAddByBtnState::ModByAddBy;
    modByAddBy2Btn.setToggleState(true, juce::dontSendNotification);
  
    scaleFromInputText2 = parseCommaSeparatedIntegers(inputText2.getText());

    if(modByVal2 == 0) {
      return;
    }

    juce::String octaves2ValStr = octaves2Input.getText();
    // try catch to check if octave 2 input is not valid, if not set to 0
    try {
        octaves2Val = std::stoi(octaves2ValStr.toStdString());
    } 
    catch (const std::invalid_argument& e) {
        // Conversion failed, set the value to 0
        octaves2Val = 0;
    } 
    catch (const std::out_of_range& e) {
        // The converted value is out of the range of representable values, set to 0
        octaves2Val = 0;
    }

		std::vector <int> modified_vector = modAllVectorByCertainAmountAndAddByCertainAmount(scaleFromInputText2, modByVal2, addByVal2, octaves2Val, false);

    audioProcessor._midiNotes = modified_vector;

    // set up output as string with commas
    juce::String moddedByAddedByText = convertVectToCommaStr(modified_vector);
    outputText1.setText(moddedByAddedByText, juce::dontSendNotification);
}

void MidiEffectAudioProcessorEditor::dontModByAddBy2() {
    modByAddBy2State = ModByAddByBtnState::NotModByAddBy;
    modByAddBy2Btn.setToggleState(false, juce::dontSendNotification);

    std::vector<int> scaleFromInputText2 = parseCommaSeparatedIntegers(inputText2.getText());
    audioProcessor._midiNotes = scaleFromInputText2;

    outputText1.setText("", juce::dontSendNotification);

}

void MidiEffectAudioProcessorEditor::addByModBy2() {
    addByModBy2State = AddByModByBtnState::AddByModBy;
    addByModBy2Btn.setToggleState(true, juce::dontSendNotification);

    scaleFromInputText2 = parseCommaSeparatedIntegers(inputText2.getText());

    if(modByVal2 == 0) {
      return;
    }

    juce::String octaves2ValStr = octaves2Input.getText();
    // octaves2Val = std::stoi(octaves2ValStr.toStdString());
    // try catch to check if octave 2 input is not valid, if not set to 0
    try {
        octaves2Val = std::stoi(octaves2ValStr.toStdString());
    } 
    catch (const std::invalid_argument& e) {
        // Conversion failed, set the value to 0
        octaves2Val = 0;
    } 
    catch (const std::out_of_range& e) {
        // The converted value is out of the range of representable values, set to 0
        octaves2Val = 0;
    }
    
		std::vector <int> modified_vector = modAllVectorByCertainAmountAndAddByCertainAmount(scaleFromInputText2, modByVal2, addByVal2, octaves2Val, true);

    audioProcessor._midiNotes = modified_vector;

    // set up output as string with commas
    juce::String addedByModdedByText = convertVectToCommaStr(modified_vector);
    outputText1.setText(addedByModdedByText, juce::dontSendNotification);

}

void MidiEffectAudioProcessorEditor::dontAddByModBy2() {
    addByModBy2State = AddByModByBtnState::NotAddByModBy;
    addByModBy2Btn.setToggleState(false, juce::dontSendNotification);

    std::vector<int> scaleFromInputText2 = parseCommaSeparatedIntegers(inputText2.getText());
    audioProcessor._midiNotes = scaleFromInputText2;

    outputText1.setText("", juce::dontSendNotification);

}

void MidiEffectAudioProcessorEditor::rotateLeft2() {
    std::vector<int> scaleFromInputText2 = parseCommaSeparatedIntegers(inputText2.getText());

    std::rotate(scaleFromInputText2.begin(), scaleFromInputText2.begin() + 1, scaleFromInputText2.end());
    
    audioProcessor._midiNotes = scaleFromInputText2;

    juce::String rotLeftText = convertVectToCommaStr(scaleFromInputText2);
    inputText2.setText(rotLeftText, juce::dontSendNotification);

}

void MidiEffectAudioProcessorEditor::rotateRight2() {
    std::vector<int> scaleFromInputText2 = parseCommaSeparatedIntegers(inputText2.getText());

    // rotate right using reverse iterators
    std::rotate(scaleFromInputText2.rbegin(), scaleFromInputText2.rbegin() + 1, scaleFromInputText2.rend());
    
    audioProcessor._midiNotes = scaleFromInputText2;

    juce::String rotRightText = convertVectToCommaStr(scaleFromInputText2);
    inputText2.setText(rotRightText, juce::dontSendNotification);

}

void MidiEffectAudioProcessorEditor::reverseInput2() {
    reverse2State = ReverseBtnState::Reverse;
    reverse2Btn.setToggleState(true, juce::dontSendNotification);

    scaleFromInputText2 = parseCommaSeparatedIntegers(inputText2.getText());

		std::vector <int> copy;
		for(int i = 0; i < scaleFromInputText2.size(); i++){
			copy.push_back(scaleFromInputText2[i]);
		}
		// reverse
		for(int i = 0; i < scaleFromInputText2.size(); i++){
			scaleFromInputText2[i] = copy[scaleFromInputText2.size() - 1 - i];
		}

    audioProcessor._midiNotes = scaleFromInputText2;

    // set up output as string with commas
    juce::String reversedText = convertVectToCommaStr(scaleFromInputText2);
    outputText1.setText(reversedText, juce::dontSendNotification);
}

void MidiEffectAudioProcessorEditor::dontReverseInput2() {
    reverse2State = ReverseBtnState::NotReverse;
    reverse2Btn.setToggleState(false, juce::dontSendNotification);

    std::vector<int> scaleFromInputText2 = parseCommaSeparatedIntegers(inputText2.getText());
    audioProcessor._midiNotes = scaleFromInputText2;

    outputText1.setText("", juce::dontSendNotification);
}

void MidiEffectAudioProcessorEditor::invertInput2() {
    sumInv2State = SumInvBtnState::Invert;
    sumInv2Btn.setToggleState(true, juce::dontSendNotification);

    scaleFromInputText2 = parseCommaSeparatedIntegers(inputText2.getText());

    std::vector <int> changed_vector;
    int octave = 12;
    //so you want to take it down to a # in between 0-11
    //then do the sum inversion thing
    //then add back on the # of octaves that you reduced the original by
    for(int i = 0; i < scaleFromInputText2.size(); i++){
        int cur_num = scaleFromInputText2[i];
        int numOctaves = 0;
        while(cur_num >= 0){
            cur_num = cur_num - octave;
            numOctaves++;
        }
        //add 12 back and decrease # of octaves once since went one more before the while loop condition was violated
        cur_num = cur_num + octave;        
        numOctaves--;
        int inverted_num = ((sumInvNum2Val + octave) - cur_num) % octave;
        inverted_num = inverted_num + (numOctaves * octave);
        changed_vector.push_back(inverted_num);
    }

    audioProcessor._midiNotes = changed_vector;

    // set up output as string with commas
    juce::String invertedText = convertVectToCommaStr(changed_vector);
    outputText1.setText(invertedText, juce::dontSendNotification);

}

void MidiEffectAudioProcessorEditor::dontInvertInput2() {
    sumInv2State = SumInvBtnState::NotInvert;
    sumInv2Btn.setToggleState(false, juce::dontSendNotification);

    std::vector<int> scaleFromInputText2 = parseCommaSeparatedIntegers(inputText2.getText());
    audioProcessor._midiNotes = scaleFromInputText2;

    outputText1.setText("", juce::dontSendNotification);

}

void MidiEffectAudioProcessorEditor::tohIt2() {
    // ok there's like an entire class of Tower of Hanoi stuff that you need to implement and stuff...
    tohIt2BtnState = TohItBtnState::TohIt;
    tohIt2Btn.setToggleState(true, juce::dontSendNotification);

    std::vector<int> scaleFromInputText2 = parseCommaSeparatedIntegers(inputText2.getText());
    Tower_Of_Hanoi toh;
    toh.numDiscs = tohNumDiscs2Val;

    //set up original state with all discs in order on the leftest pole
		std::vector <int> start_pole;
		for(int i = 0; i < tohNumDiscs2Val; i++){
			start_pole.push_back(i + 1); //i + 1 because numbering the discs 1 through numDiscs
		}
		toh.discsOnPoles.push_back(start_pole);
		std::vector <int> empty_vector_1;
		std::vector <int> empty_vector_2;
		toh.discsOnPoles.push_back(empty_vector_1);
		toh.discsOnPoles.push_back(empty_vector_2);
		//just saving all of the states of the discs on poles in giant array to use
		toh.discsOnPolesInTime.push_back(toh.discsOnPoles);
			
		//hanoi(number of discs, start pole, other pole, goal pole) 
		toh.hanoi(tohNumDiscs2Val, 0, 1, 2);

		//interleave shuffle one by one element whatever you want to call it for all of the poles for all of the times
		std::vector <int> all_shuffled_throughout_all_of_time;
		for(int i = 0; i < toh.discsOnPolesInTime.size(); i++){
			std::vector <int> all_discs_interleaved_one_point_in_time = toh.multipleVectorsToOneVectorOneElementByOneElement(toh.discsOnPolesInTime[i], scaleFromInputText2); 
			for(int j = 0; j < all_discs_interleaved_one_point_in_time.size(); j++){
				all_shuffled_throughout_all_of_time.push_back(all_discs_interleaved_one_point_in_time[j]);
			}
		}

    audioProcessor._midiNotes = all_shuffled_throughout_all_of_time;

    // set up output as string with commas
    juce::String tohdText = convertVectToCommaStr(all_shuffled_throughout_all_of_time);
    outputText1.setText(tohdText, juce::dontSendNotification);
}

void MidiEffectAudioProcessorEditor::dontTohIt2 () {
    tohIt2BtnState = TohItBtnState::NotTohIt;
    tohIt2Btn.setToggleState(false, juce::dontSendNotification);

    // CRASHING HERE, THINKING THAT BECAUSE THE BUFFER IS TOO FULL WITH THE BIG VECTOR
    // YEA IF YOU STOP PLAYING AND THEN TOGGLE IT OFF, NO CRASH
    // SO NEED TO FIGURE OUT HOW TO STOP/EMPTY BUFFER ETC
    // BEFORE ANY CHANGE WHILE PLAY IS ON IN DAW...
    std::vector<int> scaleFromInputText2 = parseCommaSeparatedIntegers(inputText2.getText());
    audioProcessor._midiNotes = scaleFromInputText2;

    outputText1.setText("", juce::dontSendNotification);

}

void MidiEffectAudioProcessorEditor::replaceThisWithThat2() {
    applyReplacement2BtnState = ApplyReplacementBtnState::Replace;
    applyReplacement2Btn.setToggleState(true, juce::dontSendNotification);

    // std::vector<int> scaleFromInputText2 = parseCommaSeparatedIntegers(inputText2.getText());
    std::regex reg(convertVectToCommaStr(replaceThis2).toStdString()); //convert int replaceThis2 to string for regex
    std::string replacementStr = replaceWith2Input.getText().toStdString();
		std::string modifiedStr = std::regex_replace(inputText2.getText().toStdString(), reg, replacementStr);
    // juce::String juceModifiedStr = juce::String::fromStdString(modifiedStr);
    juce::String juceModifiedStr = modifiedStr;
    audioProcessor._midiNotes = parseCommaSeparatedIntegers(juceModifiedStr);

    outputText1.setText(juceModifiedStr, juce::dontSendNotification);
}

void MidiEffectAudioProcessorEditor::dontReplaceThisWithThat2() {
    applyReplacement2BtnState = ApplyReplacementBtnState::NotReplace;
    applyReplacement2Btn.setToggleState(false, juce::dontSendNotification);

    std::vector<int> scaleFromInputText2 = parseCommaSeparatedIntegers(inputText2.getText());
    audioProcessor._midiNotes = scaleFromInputText2;

    outputText1.setText("", juce::dontSendNotification);

}

// OPERATIONS THAT INVOLVE BOTH SEQUENCES
void MidiEffectAudioProcessorEditor::spliceIt() {
    spliceBtnState = SpliceBtnState::Splice;
    spliceBtn.setToggleState(true, juce::dontSendNotification);

    std::vector<int> scaleFromInputText1 = parseCommaSeparatedIntegers(inputText1.getText());
    std::vector<int> scaleFromInputText2 = parseCommaSeparatedIntegers(inputText2.getText());

    std::vector < std::vector <int> > both_vectors;
		both_vectors.push_back(scaleFromInputText1);
		both_vectors.push_back(scaleFromInputText2);

    std::vector<int> spliced_vector = multiple_vectors_to_one_vector_one_element_by_one_element(both_vectors);
    audioProcessor._midiNotes = spliced_vector;

    juce::String splicedText = convertVectToCommaStr(spliced_vector);
    outputText1.setText(splicedText, juce::dontSendNotification);

}

void MidiEffectAudioProcessorEditor::dontSpliceIt() {
    spliceBtnState = SpliceBtnState::NotSplice;
    spliceBtn.setToggleState(false, juce::dontSendNotification);

    // not changing the audioProcessor buffer notes
    // just arbitrarily choosing to set output to sequence 1 since not playing spliced sequence
    // std::vector<int> scaleFromInputText1 = parseCommaSeparatedIntegers(inputText1.getText());
    // audioProcessor._midiNotes = scaleFromInputText1;

    outputText1.setText("", juce::dontSendNotification);

}

void MidiEffectAudioProcessorEditor::seq1ScalarChanged() {
    juce::String seq1ScalarStr = seq1ScalarInput.getText();
    seq1ScalarVal = std::stoi(seq1ScalarStr.toStdString());
}

void MidiEffectAudioProcessorEditor::seq2ScalarChanged() {
    juce::String seq2ScalarStr = seq2ScalarInput.getText();
    seq2ScalarVal = std::stoi(seq2ScalarStr.toStdString());
}

void MidiEffectAudioProcessorEditor::modByMultSeqsChanged() {
    juce::String modByMultSeqsStr = modByMultSeqsInput.getText();
    modByMultSeqsVal = std::stoi(modByMultSeqsStr.toStdString());
}

void MidiEffectAudioProcessorEditor::octavesMultSeqsChanged() {
    juce::String octavesMultSeqsStr = octavesMultSeqsInput.getText();
    octavesMultSeqsVal = std::stoi(octavesMultSeqsStr.toStdString());
}

void MidiEffectAudioProcessorEditor::multiplySequencesByScalarsAddTogetherModBy() {
    multiplySequencesByScalarsAddTogetherModByBtnState = MultiplySequencesByScalarsAddTogetherModByBtnState::MultiplySequencesByScalarsAddTogetherModBy;
    multiplySequencesByScalarsAddTogetherModByBtn.setToggleState(true, juce::dontSendNotification);

    std::vector<int> scaleFromInputText1 = parseCommaSeparatedIntegers(inputText1.getText());
    std::vector<int> scaleFromInputText2 = parseCommaSeparatedIntegers(inputText2.getText());

		//if vectors are not the same size it will truncate the longer one to the shorter one's size
		std::vector<int> modified_vector = add_two_vectors_ability_to_mult_by_scalars_too(scaleFromInputText1, seq1ScalarVal, scaleFromInputText2, seq2ScalarVal, modByMultSeqsVal, octavesMultSeqsVal);
    audioProcessor._midiNotes = modified_vector;

    juce::String modifiedText = convertVectToCommaStr(modified_vector);
    outputText1.setText(modifiedText, juce::dontSendNotification);

}

void MidiEffectAudioProcessorEditor::dontMultiplySequencesByScalarsAddTogetherModBy() {
    multiplySequencesByScalarsAddTogetherModByBtnState = MultiplySequencesByScalarsAddTogetherModByBtnState::NotMultiplySequencesByScalarsAddTogetherModBy;
    multiplySequencesByScalarsAddTogetherModByBtn.setToggleState(false, juce::dontSendNotification);

    outputText1.setText("", juce::dontSendNotification);
}

void MidiEffectAudioProcessorEditor::buttonClicked(juce::Button* button) {
    // SEQUENCE 1
    // reverse button 
    if(button == &reverse1Btn) {
      if(reverse1State == ReverseBtnState::NotReverse) {
        reverseInput1();
      }
      else {
        dontReverseInput1();
      }
    }

    // invert button
    if(button == &sumInv1Btn) {
      if(sumInv1State == SumInvBtnState::NotInvert) {
        invertInput1();
      }
      else {
        dontInvertInput1();
      }
    }

    // mod by add by button
    if(button == &modByAddBy1Btn) {
      if(modByAddBy1State == ModByAddByBtnState::NotModByAddBy) {
        modByAddBy1();
      }
      else {
        dontModByAddBy1();
      }
    }

    // add by mod by button
    if(button == &addByModBy1Btn) {
      if(addByModBy1State == AddByModByBtnState::NotAddByModBy) {
        addByModBy1();
      }
      else {
        dontAddByModBy1();
      }
    }

    // tower of hanoi it button
    if(button == &tohIt1Btn) {
      if(tohIt1BtnState == TohItBtnState::NotTohIt) {
        tohIt1();
      }
      else {
        dontTohIt1();
      }
    }

    // rotate left button
    if(button == &rotLeft1Btn) {
      rotateLeft1();
    }

    // rotate right button
    if(button == &rotRight1Btn) {
      rotateRight1();
    }

    // replace this with that button
    if(button == &applyReplacement1Btn) {
      if(applyReplacement1BtnState == ApplyReplacementBtnState::NotReplace) {
        replaceThisWithThat1();
      }
      else {
        dontReplaceThisWithThat1();
      }
    }


    // SEQUENCE 2
    // reverse button 
    if(button == &reverse2Btn) {
      if(reverse2State == ReverseBtnState::NotReverse) {
        reverseInput2();
      }
      else {
        dontReverseInput2();
      }
    }

    // invert button
    if(button == &sumInv2Btn) {
      if(sumInv2State == SumInvBtnState::NotInvert) {
        invertInput2();
      }
      else {
        dontInvertInput2();
      }
    }

    // mod by add by button
    if(button == &modByAddBy2Btn) {
      if(modByAddBy2State == ModByAddByBtnState::NotModByAddBy) {
        modByAddBy2();
      }
      else {
        dontModByAddBy2();
      }
    }

    // add by mod by button
    if(button == &addByModBy2Btn) {
      if(addByModBy2State == AddByModByBtnState::NotAddByModBy) {
        addByModBy2();
      }
      else {
        dontAddByModBy2();
      }
    }

    // tower of hanoi it button
    if(button == &tohIt2Btn) {
      if(tohIt2BtnState == TohItBtnState::NotTohIt) {
        tohIt2();
      }
      else {
        dontTohIt2();
      }
    }

    // rotate left button
    if(button == &rotLeft2Btn) {
      rotateLeft2();
    }

    // rotate right button
    if(button == &rotRight2Btn) {
      rotateRight2();
    }

    // replace this with that button
    if(button == &applyReplacement2Btn) {
      if(applyReplacement2BtnState == ApplyReplacementBtnState::NotReplace) {
        replaceThisWithThat2();
      }
      else {
        dontReplaceThisWithThat2();
      }
    }

    // splice both sequences button
    if(button == &spliceBtn) {
      if(spliceBtnState == SpliceBtnState::NotSplice) {
        spliceIt();
      }
      else {
        dontSpliceIt();
      }
    }

    // multiply sequences by scalars add together mod by button
    if(button == &multiplySequencesByScalarsAddTogetherModByBtn) {
      if(multiplySequencesByScalarsAddTogetherModByBtnState == MultiplySequencesByScalarsAddTogetherModByBtnState::NotMultiplySequencesByScalarsAddTogetherModBy) {
        multiplySequencesByScalarsAddTogetherModBy();
      }
      else {
        dontMultiplySequencesByScalarsAddTogetherModBy();
      }
    }

}

juce::String MidiEffectAudioProcessorEditor::convertVectToCommaStr(std::vector<int> nums) {
    // set up output as string with commas
    juce::String convertedText;
    for (int i : nums) {
        convertedText += juce::String(i) + ", ";
    }
    
    convertedText = convertedText.trimCharactersAtEnd(", "); // Remove the last comma
    
    return convertedText;
}

std::vector<int> MidiEffectAudioProcessorEditor::parseCommaSeparatedIntegers(const juce::String& str)
{
    std::vector<int> result;

    juce::StringArray tokens;
    tokens.addTokens(str, ",", "");

    for (const juce::String& token : tokens)
    {
        // trying to not add if the value is not parsed as an int
        // getIntValue() just seems to return 0 though when it's a word or nan 
        //auto an_int_perhaps = token.getIntValue();
        //if (check_if_int(an_int_perhaps)) {
        //    result.push_back(token.getIntValue());
        //}

        result.push_back(token.getIntValue());
    }

    return result;
}

// adding parameter to take # of octaves to add to result
std::vector <int> MidiEffectAudioProcessorEditor::modAllVectorByCertainAmountAndAddByCertainAmount(std::vector <int> theVector, int modBy, int addTo, int numOctaves, bool addFirst){
  //try, catch still doesn't prevent program from crashing when the mod by box is empty or contains 0
	//checking if modBy is 0 in OnSeq1ModByAddByButtonClicked and OnSeq2ModByAddByButtonClicked does the trick
  // THE ABOVE NOTE IS FROM THE WXWIDGETS STUFF NOT SURE IF IT APPLIES HERE...
	try{
		std::vector <int> ret_vect;
    int octave = 12;

    //so you want to take it down to a # in between 0-11
    //then do the mod by add by thing
    //then add back on the # of octaves that you reduced the original by
		// if(addFirst){
    for(int i = 0; i < theVector.size(); i++){
      int cur_num = theVector[i];
      int modified_num;
      if(addFirst) {
          modified_num = (cur_num + addTo) % modBy;
      }
      else {
          modified_num = (cur_num % modBy) + addTo;
      }

      modified_num = modified_num + (numOctaves * octave);

      ret_vect.push_back(modified_num);
    }

		return ret_vect;
	}
	catch(...){
		std::cout << "exception, you did something wrong" << std::endl;
	}

}

// pass in however many vectors in a 2D vector of all of them
// and returns 1 vector that pulls from the front of each until all have been used
// input vectors can be different sizes
std::vector<int> MidiEffectAudioProcessorEditor::multiple_vectors_to_one_vector_one_element_by_one_element(std::vector < std::vector <int> > all_vectors) {
    std::vector <int> shuffled_together_vector;
    int num_vectors = all_vectors.size();
    std::vector <int> num_elements_each_vector;
    //get size of each vector
    for(int i = 0; i < num_vectors; i++){
        num_elements_each_vector.push_back(all_vectors[i].size());
    }

    while(true){
        //check if no elements are left in any vector
        bool no_elements = true;
        for(int i = 0; i < num_vectors; i++){
            if(num_elements_each_vector[i] > 0){
                no_elements = false;
            }
        }
        if(no_elements){ //used all of the elements from all of the vectors
            break;
        }
        for(int i = 0; i < num_vectors; i++){
            if(num_elements_each_vector[i] > 0){
                shuffled_together_vector.push_back(all_vectors[i][0]);
                all_vectors[i].erase(all_vectors[i].begin());
                num_elements_each_vector[i]--;
            }
        }
    }

    return shuffled_together_vector;

}

std::vector<int> MidiEffectAudioProcessorEditor::add_two_vectors_ability_to_mult_by_scalars_too(std::vector <int> vect_a, int scalar_a, std::vector <int> vect_b, int scalar_b, int mod_by, int num_octaves) {
  std::vector <int> ret_vect;
  int octave = 12;
	//get smallest vector size
  int smallest_vector_size = 0;
	if(vect_a.size() < vect_b.size()){
		smallest_vector_size = vect_a.size();
	}
	else{
		smallest_vector_size = vect_b.size();
	}

	int one_result = 0;
    
  //check to that vectors are the same size just as a warning
	//already truncated larger vector if it was necessary
  if(vect_a.size() != vect_b.size()){
        std::cout << "Warning: vectors are not the same size, larger vector will be truncated to make it work" << std::endl;
	}

  //check if modding the results or not
  if(mod_by){
      for(int i = 0; i < smallest_vector_size; i++){
          one_result = (vect_a[i] * scalar_a) + (vect_b[i] * scalar_b);
          one_result = one_result % mod_by;
          // adding ability to transpose output by howeven many octaves
          one_result = one_result + (num_octaves * octave);
          ret_vect.push_back(one_result);        
      }
  }
  else{ //mod_or_not was 0 so don't mod result at all
      
      for(int i = 0; i < smallest_vector_size; i++){
          one_result = (vect_a[i] * scalar_a) + (vect_b[i] * scalar_b);
          // adding ability to transpose output by howeven many octaves
          one_result = one_result + (num_octaves * octave);
          ret_vect.push_back(one_result);        
      }

  }
  return ret_vect;
}

template <class T>
T MidiEffectAudioProcessorEditor::Tower_Of_Hanoi::popAndReturn(std::vector <T> &vect){
    T popped = vect[vect.size() - 1];
    vect.pop_back();
    return popped;
}

template <class T>
T MidiEffectAudioProcessorEditor::Tower_Of_Hanoi::popFrontAndReturn(std::vector <T> &vect){
    T popped = vect[0];
    vect.erase(vect.begin());
    return popped;
}

void MidiEffectAudioProcessorEditor::Tower_Of_Hanoi::move(int f, int t){ //f - from, t - to
    int grabbed_disc = popAndReturn(discsOnPoles[f]);
    discsOnPoles[t].push_back(grabbed_disc);

    //just saving all of the states of the discs on poles in giant vector to use 
    //c++ does copy the vector when you do this, not just point to original vector
    discsOnPolesInTime.push_back(discsOnPoles);
    //num_moves++;
    //global_num_moves++;
}

//the power is in how it's defined and how you think about that relation in general
//move f - from using h - helper to t - to
void MidiEffectAudioProcessorEditor::Tower_Of_Hanoi::hanoi(int n, int f, int h, int t){ //n - # of discs, f - from, h - helper pole (go between), t - to
    //base case
    if(n == 0){
        return;
    }

    //basically just move twice
    //from helper, then helper to the to
    //recursive call with one less
    //you want to solve the order in the h - helper pole spot for n-1 discs
    hanoi(n - 1, f, t, h);

    //then you can move the last disc to the goal target t - to
    //since already solved the order for n-1 discs at the helper position pole spot
    move(f, t); //the first time this returns it's because the subproblem is to just move the 1 disc from the start to the end

    //then move the already solved helper pole to the target t - to using the f - from pole as the helper this time
    hanoi(n - 1, h, f, t); //solving the situation with 1 less disc will need to be solved and arranged in the alternating non goal helper pole
}

std::vector <int> MidiEffectAudioProcessorEditor::Tower_Of_Hanoi::multipleVectorsToOneVectorOneElementByOneElement(std::vector < std::vector <int> > allVectorsOneTime, std::vector <int> scaleToTranslateTo){
	bool translate_to_scale = true; //set to false if you just want the index #s of discs instead, you prob would never want in this application

	std::vector <int> shuffled_together_vector;
    int num_vectors = allVectorsOneTime.size();
    std::vector <int> num_elements_each_vector;
    for(int i = 0; i < num_vectors; i++){
        num_elements_each_vector.push_back(allVectorsOneTime[i].size());
    }

    //sequence is going from pole A to B to C 
    //always picking the disc from the bottom
    //throughout all of time
    while(true){
        bool no_elements = true;
        //check to see if still an element in any vector to add, if not break
        for(int i = 0; i < num_vectors; i++){
            if(num_elements_each_vector[i] > 0){
                no_elements = false;
            }
        }
        if(no_elements){ //used all of the elements from all of the vectors
            break;
        }

        for(int i = 0; i < num_vectors; i++){
            if(num_elements_each_vector[i] > 0){
                //pop 1st element (bottom disc)
                int popped = popFrontAndReturn(allVectorsOneTime[i]); //need to pop front
                if(translate_to_scale){
                    shuffled_together_vector.push_back(scaleToTranslateTo[(popped - 1) % scaleToTranslateTo.size()]);
                }
                else{
                    shuffled_together_vector.push_back(popped);
                }

                num_elements_each_vector[i]--;
            }
        }
    }

    return shuffled_together_vector;

}

