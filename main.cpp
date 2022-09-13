#include <iostream>
#include <filesystem>
#include <array>
#include <vector>
#include <span>
#include <algorithm>
#include <string>

#include <SFML/Graphics.hpp>


class Application {
public:
    Application(sf::VideoMode videoMode) : window(videoMode, "Shader Applicator"), capture{}, rect{} {
        imageFile = findFile(FileType::Image);
        shaderFile = findFile(FileType::Shader);
        loadAssets();
    }

    enum class FileType { Image, Shader };
    enum class FindSeq { First, Next, Last };

    void loadAssets() {
        if (!texture.loadFromFile(imageFile.string()))
            std::cerr << "Texture failed to load!" << std::endl;

        if (!shader.loadFromFile(shaderFile.string(), sf::Shader::Fragment)) 
            std::cerr << "Shader failed to load!" << std::endl;
        sf::Image temp = texture.copyToImage();
        temp.flipVertically();
        texture.loadFromImage(temp);
    	shader.setUniform("texture", texture);
        shader.setUniform("resolution", sf::Glsl::Vec2(window.getSize()));

        window.setTitle("Shader Applicator : " + imageFile.stem().string() + ", " + shaderFile.stem().string());
        rect.setSize(static_cast<sf::Vector2f>(texture.getSize()));
    }

    void saveRendered() {
        std::filesystem::path newImagePath = imageFile;
        shader.setUniform("resolution", sf::Glsl::Vec2(texture.getSize()));
        capture.create(texture.getSize().x, texture.getSize().y);
        capture.clear();
        capture.draw(rect, &shader);
        capture.display();
        newImagePath.remove_filename() += imageFile.stem() += std::filesystem::path("_") += shaderFile.stem() +=imageFile.extension();
        capture.getTexture().copyToImage().saveToFile(newImagePath.string());
        shader.setUniform("resolution", sf::Glsl::Vec2(window.getSize()));
    }

    void tick() {
        for (sf::Event e; window.pollEvent(e);) {
            switch (e.type) {
            case sf::Event::EventType::Closed:
                window.close();
                break;
            case sf::Event::EventType::Resized:
                loadAssets();
                break;
            case sf::Event::EventType::KeyReleased:
                switch (e.key.code) {
                case sf::Keyboard::R:
                    loadAssets();
                    break;
                case sf::Keyboard::F:
                    window.setSize(texture.getSize());
                    break;
                case sf::Keyboard::S:
                    saveRendered();
                    break;
                case sf::Keyboard::Up:
                    shaderFile = findFile(FileType::Shader, FindSeq::Last, shaderFile);
                    loadAssets();
                    break;
                case sf::Keyboard::Down:
                    shaderFile = findFile(FileType::Shader, FindSeq::Next, shaderFile);
                    loadAssets();
                    break;
                case sf::Keyboard::Left:
                    imageFile = findFile(FileType::Image, FindSeq::Last, imageFile);
                    loadAssets();
                    break;
                case sf::Keyboard::Right:
                    imageFile = findFile(FileType::Image, FindSeq::Next, imageFile);
                    loadAssets();
                    break;
                }
                break;
            }
        }
 
        window.clear();
        window.draw(rect, &shader);
        window.display();
    }

    bool isRunning() { return window.isOpen(); }

protected:
    static std::filesystem::path findFile(const FileType type, const FindSeq seq = FindSeq::First, std::filesystem::path lastFile = "") {
        const std::filesystem::path assets{ std::filesystem::exists("assets") ? "assets" : 
                                            std::filesystem::exists("../assets") ? "../assets" : std::filesystem::current_path() };
        using namespace std::string_view_literals;
        constexpr std::initializer_list supportedImageExts{".bmp"sv, ".png"sv, ".tga"sv, ".jpg"sv, ".gif"sv, ".psd"sv, ".hdr"sv, ".pic"sv};
        constexpr std::initializer_list supportedFragExts{".frag"sv, ".fsh"sv, ".fs"sv, ".glsl"sv};
        const std::vector exts = (type == FileType::Image ? supportedImageExts : supportedFragExts);

        std::vector<std::filesystem::path> foundFiles{};
        for (const auto& entry : std::filesystem::recursive_directory_iterator{assets})  {
            if (!entry.is_regular_file())
                continue;
            if (std::ranges::find(exts, entry.path().extension()) == exts.end())
                continue;
            foundFiles.push_back(entry.path());
        }
        if (foundFiles.empty()) {
            std::cerr << "File(s) not found!" << std::endl;
            exit(-1);
        }
        if (seq == FindSeq::First || foundFiles.size() == 1)
            return foundFiles[0];
        auto idx = std::ranges::find(foundFiles, lastFile) - foundFiles.begin();
        if (idx == foundFiles.size())
            return foundFiles[0];
        idx += (seq == FindSeq::Next ? 1 : -1);
        idx = (idx + foundFiles.size()) % foundFiles.size();
        return foundFiles[idx];
    }

    sf::RenderWindow window;
    sf::RenderTexture capture;
    sf::Texture texture;
    sf::Shader shader;
    sf::RectangleShape rect;
    std::filesystem::path imageFile, shaderFile;
};


int main() {
    Application app(sf::VideoMode(800, 600));

	while (app.isRunning()) {
        app.tick();
    }

	return 0;
}
