A glsl shader applicator for images.

The app looks for assets (shaders and images) recursively in `./assets`, `../assets`, or if neither directory exists, the current working directory.

|Key  |Command                                             |
|:---:|----------------------------------------------------|
|R    |Reloads assets                                      |
|F    |Fits window to image                                |
|S    |Saves output to `<image>_<shader>.<image extension>`|
|Up   |Cycles shader to last file                          |
|Down |Cycles shader to next file                          |
|Left |Cycles image to last file                           |
|Right|Cycles iamge to next file                           |


Supported image formats: `.bmp`, `.png`, `.tga`, `.jpg`, `.gif`, `.psd`, `.hdr`, `.pic`

Supported shader extensions: `.frag`, `.fsh`, `.fs`, `.glsl`
