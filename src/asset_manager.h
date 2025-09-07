#include <filesystem>

class AssetManager {
    public:
    static void SetAssetRoot(const std::filesystem::path& root) {
        assetRoot = root;
    }

    static std::filesystem::path GetAssetPath(const std::filesystem::path& relativePath) {
        if (assetRoot.empty()) {
            return relativePath; // no root set, return as-is
        }
        return assetRoot / relativePath;
    }

    private:
        inline static std::filesystem::path assetRoot;
};