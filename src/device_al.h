#ifndef DEVICE_AL_HPP
#define DEVICE_AL_HPP


#include <list>
#include <AL/al.h>
#include <AL/alc.h>
#include "audiere.h"
#include "internal.h"
#include "utility.h"


namespace audiere {

  class ALOutputStream;


  class ALAudioDevice : public RefImplementation<AudioDevice> {
  public:
    static ALAudioDevice* create(const ParameterList& list);

  private:
    ALAudioDevice(ALCdevice* device, ALCcontext* context);
    ~ALAudioDevice();

  public:
    void update();
    OutputStream* openStream(SampleSource* source);
    OutputStream* openBuffer(
      void* samples, int sample_count,
      int channel_count, int sample_rate, SampleFormat sample_format);

  private:
    void removeStream(ALOutputStream* stream);

    ALCdevice*  m_device;
    ALCcontext* m_context;

    typedef std::list<ALOutputStream*> StreamList;
    StreamList m_open_streams;

    friend class ALOutputStream;
  };


  class ALOutputStream : public RefImplementation<OutputStream> {
  public:
    void  play();
    void  stop();
    bool  isPlaying();
    void  reset();
    void  setRepeat(bool repeat);
    bool  getRepeat();
    void  setVolume(float volume);
    float getVolume();
    void  setPan(float pan);
    float getPan();

    bool isSeekable();
    int getLength();
    void setPosition(int position);
    int getPosition();

  private:
    ALOutputStream(
      ALAudioDevice* device,
      SampleSource* source,
      ALuint al_source,
      ALuint* buffers,
      ALenum format,
      int sample_rate);
    ~ALOutputStream();

    void update();
    int read(void* samples, int sample_count);
    void fillBuffers();

  private:
    RefPtr<ALAudioDevice> m_device;
    RefPtr<SampleSource> m_source;

    // informational (convenience)
    ALenum m_format;
    int    m_sample_size;  // (num channels * bits per sample / 8)
    int    m_sample_rate;

    // the last sample read
    ALubyte* m_last_sample;  
  
    // AL objects
    ALuint  m_ALsource;
    ALuint* m_buffers;

    int m_buffer_length;  // in samples
    bool m_is_playing;
    float m_volume;

    friend class ALAudioDevice;
  };

}


#endif