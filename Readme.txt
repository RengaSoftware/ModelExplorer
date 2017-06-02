This plugin allows you to browse Renga model, obtain the object parameters and quantities, change values of custom user-defined object attributes.

To build the plugin you need RengaSDK and Qt 5.8.1.
You can download the latest version of RengaSDK from "http://rengacad.com/ru/support/sdk/".
There are two environment variables (renga_sdk, qt5_path) used in plugin.
You should add these variables to the system, and specify paths to RengaSDK folder and Qt folder accordingly.
Alternatively you can set required paths in Visual Studio.

You also can download latest builded plugin from "https://github.com/RengaSoftware/ModelExplorer".

To make plugin working follow next steps:

1. Open Renga installation folder.
2. Open Plugins directory (create in case of missing).
3. Create directory ModelExplorerPlugin and open it.
4. Copy plugin files to the just created directory. If you build plugin by your own - all required files located in project output directory.
5. Restart Renga.

Feel free to use this plugin for work, as a basis for your own development or as an example.