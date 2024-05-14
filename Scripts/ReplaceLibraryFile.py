from distutils.dir_util import copy_tree
from time import sleep
import shutil
import json
import sys
import os

def work_path():
    script_path_list = __file__.split("\\")
    script_path_list.pop()
    script_path_list.pop()
    
    directory_path = ""
    for directory in script_path_list:
        directory_path += directory + "\\"

    return directory_path

def main():
    try:
        propsFile = open(work_path() + "Scripts\\LibraryLocation.json", "r")
        props = json.load(propsFile)
        propsFile.close()

        if(os.path.exists(props["LibraryLocation"] + "Library/")):
            shutil.rmtree(props["LibraryLocation"] + "Library/")
        os.mkdir(props["LibraryLocation"] + "Library/")

        if(os.path.exists(props["LibraryLocation"] + "Include/")):
            shutil.rmtree(props["LibraryLocation"] + "Include/")

        # Get all platform options
        platform_options = []
        platform_paths = []
        platform_short = []

        if os.path.exists(work_path() + "Build/x64-release/Library/AQC.lib"):
            platform_options.append("x64 Release")
            platform_paths.append(work_path() + "Build/x64-release/Library/AQC.lib")
            platform_short.append("x64")

        if os.path.exists(work_path() + "Build/x64-debug/Library/AQC.lib"):
            platform_options.append("x64 Debug")
            platform_paths.append(work_path() + "Build/x64-debug/Library/AQC.lib")
            platform_short.append("x64")

        if len(platform_options) > 0:
            resposta = 0
            if len(sys.argv) > 1:
                if sys.argv[1] == "Debug":
                    resposta = 1

            if resposta < len(platform_paths) and resposta >= 0:
                shutil.copyfile(platform_paths[resposta], props["LibraryLocation"] + f"Library/AQC.{platform_short[resposta]}.lib")
                copy_tree(work_path() + "Include\\", props["LibraryLocation"] + "Include/")
                print("\n[RESULTADO] Operacao realizada com sucesso.")
            else:
                print("\n[ERRO] Opcao invalida.")
        else:
            print("[ERRO] Compile a biblioteca em uma plataforma valida.")

    except FileNotFoundError:
        print("[ERRO] O arquivo de configuracoes Scripts\\LibraryLocation.json nao existe.")

    sleep(3.0)
    
if __name__ == "__main__":
    main()