#pragma once
#include "sid-api/libcsid.h"
#include <SDL_audio.h>
#include <SDL_mixer.h>
#include <thread>
using namespace std::chrono_literals;

class sid_metadata
{
public:
    std::string author{};
    std::string title{};
    std::string info{};
    u8 total_tunes{};
    u8 default_tune{};
};
enum class eC64Model : s16
{
    mPerfered = 0,
    m6581 = 6581,
    m8580 = 8580,
};
class sid_subtune
{
public:
    sid_subtune() = default;
    sid_subtune(s8 s, float v) :
        id(s), volume(v)
    {}
    s8 id{ -1 };
    float volume{ 1.f };
    bool active{ false };
};
class sid
{
public:
    ~sid()
    {
        reset();
    }
    static void reset(bool soft = false)
    {
        csid.reset();
        if (soft)
        {
            m_current_subtune = {};
        }
        SDL_CloseAudio();
    }
    static void play(void* userdata, u8* stream, int len)
    {
        if (!m_current_subtune.active)
        {
            std::cout << "SDL is requesting a sample while paused!" << std::endl;
            return;
        }
        csid.set_volume(m_current_subtune.volume);
        csid.render(stream, len); 
        set_end_time();
    }
    void set_data(eC64Model model, int samplerate, int subtune)
    {
        // The user loaded a file while it was still playing, so we need to reset our time.
        if (m_current_subtune.active)
        {
            set_start_time();
        }
        m_model = model;
        m_samplerate = samplerate;
        m_current_subtune.id = subtune;
    }
    void load(const std::string& name, u8* buffer, size_t buffer_size)
    {
        csid.init_helper(m_samplerate, static_cast<s16>(m_model));
        memcpy(m_filedata, buffer, buffer_size);
        csid.load(buffer, buffer_size, m_current_subtune.id);
        metadata.author = (char*)&csid.SIDauthor;
        metadata.title = (char*)&csid.SIDtitle;
        metadata.info = (char*)&csid.SIDinfo;
        metadata.total_tunes = csid.total_tunes;
        metadata.default_tune = csid.default_tune;
    }
    void set_volume_level(float level)
    {
        m_current_subtune.volume = level;
    }
    void play()
    {
        set_start_time();
        m_current_subtune.active = true;
        SDL_PauseAudio(0);
    }
    void pause()
    {
        set_end_time();
        m_current_subtune.active = false;
        SDL_PauseAudio(1);
    }
    s8 setup_sdl()
    {          
        if (m_current_subtune.active || m_current_subtune.id != -1)
        {
            // Already created our handles, but we want a new sample rate          
            SDL_CloseAudio();
        }
        if (SDL_Init(SDL_INIT_AUDIO) < 0)
        {
            std::cerr << "Couldn't initialize SDL:" << SDL_GetError() << std::endl;
            return 1;
        }
        SDL_AudioSpec soundspec{};
        soundspec.freq = m_samplerate;
        soundspec.channels = 1;
        soundspec.format = AUDIO_S16;
        soundspec.samples = 16384;
        soundspec.userdata = NULL;
        soundspec.callback = &sid::play;
        if (SDL_OpenAudio(&soundspec, NULL) < 0)
        {
            std::cerr << "Couldn't open audio:" << SDL_GetError() << std::endl;
            return 2;
        }
        if (m_autostart_on_load)
        {
            play();
        }
        return 0;
    }
    static void set_start_time()
    {
        m_time_start = std::chrono::high_resolution_clock::now();
    }
    static void set_end_time()
    {
        m_time_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed{ m_time_end - m_time_start };
        m_time_elapsed = elapsed.count();
    }
    static double m_time_elapsed;
    static std::chrono::steady_clock::time_point m_time_start;
    static std::chrono::steady_clock::time_point m_time_end;
    static libcsid csid;
    static sid_subtune m_current_subtune;
    int m_frequency{};
    bool m_autostart_on_load{ true };
    eC64Model m_model{ eC64Model::m6581 };
    s32 m_samplerate{ DEFAULT_SAMPLERATE };
    size_t m_filesize{};
    u8 m_filedata[MAX_DATA_LEN];
    sid_metadata metadata;
};
inline std::chrono::steady_clock::time_point sid::m_time_start{};
inline std::chrono::steady_clock::time_point sid::m_time_end{};
inline double sid::m_time_elapsed{};
inline libcsid sid::csid{};
inline sid_subtune sid::m_current_subtune{};