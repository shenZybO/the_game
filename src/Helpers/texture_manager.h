#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include "raylib.h"

/**
 * @brief Global texture manager that loads and caches textures by file name.
 *
 * Use GetTexture() to retrieve a Texture2D. If it is not loaded yet, it will be
 * loaded, cached and then returned. Returned references are non-owning; call
 * UnloadAll() once at shutdown to free all textures.
 */
class TextureManager {
  public:
    static TextureManager& Instance();

    /**
     * @brief Get a texture for the given file (relative asset path or name).
     * If not loaded yet, it will be loaded and cached.
     *
     * @param fileName File name/path identifying the texture (used as key).
     * @return Texture2D& Non-owning reference to the cached texture.
     */
    Texture2D& GetTexture(std::string_view fileName);

    /**
     * @brief Unload and clear all cached textures. Call before CloseWindow().
     */
    void UnloadAll();

  private:
    TextureManager() = default;
    ~TextureManager();
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    std::unordered_map<std::string, Texture2D> cache; // keyed by file name
};
