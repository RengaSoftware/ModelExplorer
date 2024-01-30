------------------------------------------------------------
ModelExplorer - The Renga plugin for browsing and editing Renga model
------------------------------------------------------------
**[Download](https://github.com/RengaSoftware/ModelExplorer/releases)**

About
-----

This plugin allows you to browse Renga building model and assembly models, obtain parameters and quantities of the object, style, material etc., change values of user-defined object properties, set visibility state to model objects, select model object from the tree view.

How to install the plugin
-----

1. Open Renga installation folder.
2. Open Plugins directory (create in case of missing).
3. Create directory ModelExplorerPlugin and open it.
4. Copy plugin files to the just created directory. If you build plugin by your own - all required files located in project output directory.
5. Restart Renga.

How to build the plugin
-----

To build the plugin you need RengaSDK and Qt 5.9.3.
You can download the latest version of RengaSDK from "https://rengabim.com/sdk/".
There are two environment variables (renga_com_sdk, qt5_path) used in the plugin.
You should add these variables to the system, and specify paths to RengaSDK folder and Qt folder accordingly.
Alternatively you can set required paths in Visual Studio.

Other links
-----

- [Renga system](http://rengacad.com/)
- [ModelExplorer issue tracker](https://github.com/RengaSoftware/ModelExplorer/issues)


Feel free to use this plugin for work, as a basis for your own development or as an example.
