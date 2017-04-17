#include "MyFirstPlugin.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

const int kNumPrograms = 1;

//Important for new install on computer:
//Open Explorer and go to C : \Program Files\.
//Create a new folder and call it vsthost.Now download Savihost
//(you’ll want the savihostx86.zip) and unzip it.
//Copy the savihost.exe into the folder you just created :

enum EParams
{
  kGain = 0,
  kNumParams
};

enum ELayout
{
  kWidth = GUI_WIDTH,
  kHeight = GUI_HEIGHT,

  kGainX = 100,
  kGainY = 100,
  kKnobFrames = 60
};

MyFirstPlugin::MyFirstPlugin(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo), mGain(1.)
{
  TRACE;

  //arguments are: name, defaultVal, minVal, maxVal, step, label
  GetParam(kGain)->InitDouble("Gain", 50., 0., 100.0, 0.01, "%");
  GetParam(kGain)->SetShape(1.);

  IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
  pGraphics->AttachPanelBackground(&COLOR_RED);

  IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, kKnobFrames);

  pGraphics->AttachControl(new IKnobMultiControl(this, kGainX, kGainY, kGain, &knob));

  AttachGraphics(pGraphics);

  //MakePreset("preset 1", ... );
  MakeDefaultPreset((char *) "-", kNumPrograms);
}

MyFirstPlugin::~MyFirstPlugin() {}

void MyFirstPlugin::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
  // Mutex is already locked for us. 
	/*In computer science, a lock or mutex(from mutual exclusion) 
		is a synchronization mechanism for enforcing limits on access 
		to a resource in an environment where there are many threads 
		of execution.A lock is designed to enforce a mutual exclusion 
		concurrency control policy.*/  

  double* in1 = inputs[0];
  double* in2 = inputs[1];
  double* out1 = outputs[0];
  double* out2 = outputs[1];

  /*in1 points to the first sequence of samples(left channel), 
  in2 to the samples for the right channel.
	  After doing the same for the output buffer, 
	  we can iterate over the input and output buffers to process them*/


  /*For every sample, we take the input value, 
	  multiply it by mGain and write it to the output buffer.
	  nFrames tells us how many samples there are per channel 
	  so we know how long the buffers are.*/
  for (int s = 0; s < nFrames; ++s, ++in1, ++in2, ++out1, ++out2)
  {
    *out1 = *in1 * mGain;
    *out2 = *in2 * mGain;
  }
}

void MyFirstPlugin::Reset()
{
  TRACE;
  IMutexLock lock(this);
}

void MyFirstPlugin::OnParamChange(int paramIdx)
{
  IMutexLock lock(this);

  switch (paramIdx)
  {
    case kGain:
      mGain = GetParam(kGain)->Value() / 100.;
      break;

    default:
      break;
  }
}
