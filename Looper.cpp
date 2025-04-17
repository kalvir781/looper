#include "daisysp.h"
#include "daisy_pod.h"

#define MAX_SIZE (48000 * 60 * 5) // 5 minutes of floats at 48 khz

using namespace daisysp;
using namespace daisy;

static DaisyPod pod;

bool first = true;  //first loop (sets length)
bool rec   = false; //currently recording
bool play  = false; //currently playing

int                 pos = 0;
float DSY_SDRAM_BSS buf[MAX_SIZE];
int                 mod    = MAX_SIZE;
int                 len    = 0;
bool                res    = false;

void ResetBuffer();
void Controls();

void NextSamples(float&                               output,
                 AudioHandle::InterleavingInputBuffer in,
                 size_t                               i);

static void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t                                size)
{
    float output = 0;

    Controls();

    for(size_t i = 0; i < size; i += 2)
    {
        NextSamples(output, in, i);

        // left and right outs
        out[i] = out[i + 1] = output;
    }
}

int main(void)
{
    // initialize pod hardware and oscillator daisysp module

    pod.Init();
    pod.SetAudioBlockSize(4);
    ResetBuffer();

    // start callback
    pod.StartAdc();
    pod.StartAudio(AudioCallback);

    while(1) {}
}

//Resets the buffer
void ResetBuffer()
{
    play  = false;
    rec   = false;
    first = true;
    pos   = 0;
    len   = 0;
    for(int i = 0; i < mod; i++)
    {
        buf[i] = 0;
    }
    mod = MAX_SIZE;
}

void UpdateButtons()
{
    //button1 pressed
    if(pod.button2.RisingEdge())
    {
        if(first && rec)
        {
            first = false;
            mod   = len;
            len   = 0;
        }

        res  = true;
        play = true;
        rec  = !rec;
    }

    //button1 held
    if(pod.button2.TimeHeldMs() >= 1000 && res)
    {
        ResetBuffer();
        res = false;
    }

    //button2 pressed and not empty buffer
    if(pod.button1.RisingEdge() && !(!rec && first))
    {
        play = !play;
        rec  = false;
    }
}

//Deals with analog controls
void Controls()
{
    pod.ProcessAnalogControls();
    pod.ProcessDigitalControls();

    UpdateButtons();

    //leds
    pod.led1.Set(0, 0, play == true);
    pod.led2.Set(0, rec == true, 0);

    pod.UpdateLeds();
}

void WriteBuffer(AudioHandle::InterleavingInputBuffer in, size_t i)
{
    buf[pos] = buf[pos] + in[i];
    if(first)
    {
        len++;
    }
}

void NextSamples(float&                               output,
                 AudioHandle::InterleavingInputBuffer in,
                 size_t                               i)
{
    if(rec)
    {
        WriteBuffer(in, i);
    }

    output = buf[pos];

    //automatic looptime
    if(len >= MAX_SIZE)
    {
        first = false;
        mod   = MAX_SIZE;
        len   = 0;
    }

    if(play)
    {
        pos++;
        pos %= mod;
    }

    if(!rec)
    {
        output = output + in[i];
    }
}
