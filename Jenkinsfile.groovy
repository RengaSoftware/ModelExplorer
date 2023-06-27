import java.nio.file.Paths
// Requirements:
// agent: Visual Studio 2022 with component "Desktop development with C++" (Microsoft.VisualStudio.Workload.VCTools)
// Jenkins instance: Multibranch pipeline plugin, File operations plugin.

def msBuildExe = 'C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/MSBuild/Current/Bin/MSBuild.exe'
def rengaSdkUrl = 'https://dl.rengabim.com/SDK/RengaSDK.zip'
def rengaSdkZip = 'RengaSDK.zip'
def rengaSdkDir = 'RengaSDK'
def platform = 'x64'
def nodeLabel = 'qt'

node(nodeLabel) {
    // initialize repo
    checkout scm
    bat(script: 'git clean -fdx --quiet')

    // download Renga SDK
    fileOperations([
            fileDownloadOperation(url: rengaSdkUrl, targetFileName: rengaSdkZip,
                    targetLocation: '.', userName: '', password: '')
    ])
    unzip(zipFile: rengaSdkZip, dir: rengaSdkDir, quiet: true)

    // build ModelExplorer plugin
    withEnv(["renga_com_sdk=${WORKSPACE}\\${rengaSdkDir}"]) {
        bat(encoding: 'cp866', script: "\"${msBuildExe}\" ModelExplorerPlugin.sln \"-p:Configuration=Release;Platform=${platform}\"")
    }

    // archive files as artifacts
    def outDir = "ModelExplorerPlugin_${env.BRANCH_NAME}_${env.BUILD_NUMBER}"
    def outZip = "${outDir}.zip"
    fileOperations([
            folderCreateOperation(folderPath: outDir),
            fileCopyOperation(
                    targetLocation: outDir,
                    flattenFiles: true,
                    includes: [
                            "Readme.txt",
                            "License.txt",
                            "Release/${platform}/ModelExplorerPlugin.rne",
                            "ModelExplorerPlugin/modelexplorerplugin_ru.qm",
                            "ModelExplorerPlugin/modelexplorerplugin_en.qm",
                            "ModelExplorerPlugin/Resources/ModelExplorerPlugin.rndesc",
                            "ModelExplorerPlugin/Resources/ModelExplorerIcon.png",
                    ].join(','),
            ),
    ])
    zip(zipFile: outZip, dir: outDir, archive: true)
}