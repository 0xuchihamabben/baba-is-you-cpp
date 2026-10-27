#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <map>
#include <string>

namespace Baba {

class AssetManager {
  public:
    AssetManager(AssetManager &) = delete;
    static AssetManager &Get() {
        static AssetManager instance;
        return instance;
    }

    void LoadTexture(const std::string &name, const std::string &path) {
        sf::Texture tex;
        if (tex.loadFromFile(path)) {
            m_textures[name] = std::move(tex);
        }
    }

    void LoadFont(const std::string &name, const std::string &path) {
        sf::Font font;
        if (font.openFromFile(path)) {
            m_fonts[name] = std::move(font);
        }
    }

    void LoadSoundBuffer(const std::string &name, const std::string &path) {
        sf::SoundBuffer buffer;
        if (buffer.loadFromFile(path)) {
            m_soundBuffers[name] = buffer;
        }
    }

    const sf::Texture &GetTexture(const std::string &name) const { return m_textures.at(name); }

    const sf::Font &GetFont(const std::string &name) const { return m_fonts.at(name); }

    const sf::SoundBuffer &GetSoundBuffer(const std::string &name) const {
        return m_soundBuffers.at(name);
    }

    void LoadGameAssets() {
        LoadTexture("tiled", "assets/objects_tiled.png");

        LoadTexture("baba_pic", "assets/baba_avatar.png");
        LoadTexture("flag_pic", "assets/flag_pic.png");
        LoadTexture("rock_pic", "assets/movable_rock.png");

        LoadTexture("text_baba", "assets/red_baba_bloc.png");
        LoadTexture("text_flag", "assets/flag_bloc.png");
        LoadTexture("text_wall", "assets/WALL_WORD.png");
        LoadTexture("wall_obj", "assets/WALL_BLOC.png");
        LoadTexture("text_rock", "assets/rock_BLOC.png");
        LoadTexture("text_is", "assets/IS_BLOC.png");
        LoadTexture("text_you", "assets/YOU_BLOC.png");
        LoadTexture("text_win", "assets/WIN_BLOC.png");
        LoadTexture("text_stop", "assets/STOP_BLOC.png");
        LoadTexture("text_push", "assets/PUSH_bloc.png");

        LoadTexture("water_obj", "assets/Water.png");
        LoadTexture("text_water", "assets/Water_WORD.png");
        LoadTexture("text_sink", "assets/sink_word.png");

        LoadFont("main_font", "assets/FreeSans.ttf");

        LoadSoundBuffer("win_sound", "assets/victory.mp3");
        LoadSoundBuffer("lose_sound", "assets/lose.mp3");
    }

    void Clear() {
        m_textures.clear();
        m_fonts.clear();
        m_soundBuffers.clear();
    }

  private:
    AssetManager() = default;
    ~AssetManager() = default;
    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, sf::Font> m_fonts;
    std::map<std::string, sf::SoundBuffer> m_soundBuffers;
};

}
