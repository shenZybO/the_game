#include "texture_manager.h"
#include "asset_manager.h"
#include <utility>

TextureManager& TextureManager::Instance() {
    static TextureManager instance;
    return instance;
}

Texture2D& TextureManager::GetTexture(std::string_view fileName) {
    auto it = cache.find(std::string(fileName));
    if (it != cache.end()) return it->second;

    // Resolve asset path and load
    auto fullPath = AssetManager::GetAssetPath(std::string(fileName));
    Texture2D tex = LoadTexture(fullPath.string().c_str());
    // Optional: log errors (raylib returns id=0 on failure)
    if (tex.id == 0) {
        TraceLog(LOG_ERROR, "TextureManager: Failed to load texture: %s", fullPath.string().c_str());
    } else {
        TraceLog(LOG_INFO, "TextureManager: Loaded texture: %s (id=%u)", fullPath.string().c_str(), tex.id);
    }
    auto [insIt, _] = cache.emplace(std::string(fileName), tex);
    return insIt->second;
}

void TextureManager::UnloadAll() {
    if (cache.empty()) return;  // cache empty, nothing to unload
    for (auto& kv : cache) {
        if (kv.second.id != 0) {
            UnloadTexture(kv.second);
        }
    }
    cache.clear();
}

TextureManager::~TextureManager() {
    // Guard: raylib requires window/rlgl context alive to unload; prefer explicit UnloadAll().
    // As a fallback, attempt to unload if user forgot.
    if (!cache.empty()) {
        UnloadAll();
    }
}
