#include <filesystem>

/**
 * @brief Simple asset manager that resolves asset paths relative to a root.
 *
 * The class stores a process-global asset root path which is prepended to
 * requested relative asset paths. When no root is set, the input path is
 * returned unchanged.
 */
class AssetManager {
    public:
    /**
     * @brief Set the root folder for asset resolution.
     *
     * Typically called once at program startup to set assets relative to the
     * executable or resource folder.
     *
     * @param root Filesystem path to the asset root.
     */
    static void SetAssetRoot(const std::filesystem::path& root) {
        assetRoot = root;
    }

    /**
     * @brief Convert a relative asset path into an absolute path under the configured root.
     *
     * If no root has been set the relative path is returned unchanged.
     *
     * @param relativePath Path relative to the asset root.
     * @return std::filesystem::path Resolved path.
     */
    static std::filesystem::path GetAssetPath(const std::filesystem::path& relativePath) {
        if (assetRoot.empty()) {
            return relativePath; // no root set, return as-is
        }
        return assetRoot / relativePath;
    }

    private:
        inline static std::filesystem::path assetRoot;
};