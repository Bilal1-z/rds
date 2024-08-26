
OS=""
case "$(uname -s)" in
    Linux*)     OS=Linux;;
    Darwin*)    OS=Mac;;
    CYGWIN*|MINGW32*|MSYS*) OS=Windows;;
    *)          OS="UNKNOWN";;
esac


export MY_INSTALL_DIR=$HOME/.local
mkdir -p $MY_INSTALL_DIR


export PATH="$MY_INSTALL_DIR/bin:$PATH"


if ! command -v cmake &> /dev/null || [ "$(cmake --version | cut -d ' ' -f 3)" \< "3.13" ]; then
    echo "Installing cmake..."
    case $OS in
        Linux)
            sudo apt install -y cmake
            
            if [ "$(cmake --version | cut -d ' ' -f 3)" \< "3.13" ]; then
                wget -q -O cmake-linux.sh https://github.com/Kitware/CMake/releases/download/v3.19.6/cmake-3.19.6-Linux-x86_64.sh
                sh cmake-linux.sh -- --skip-license --prefix=$MY_INSTALL_DIR
                rm cmake-linux.sh
            fi
            ;;
        Mac)
            brew install cmake
            ;;
        *)
            echo "Error: Unsupported OS for cmake installation."
            exit 4
            ;;
    esac
fi




DIR="build"


if [ ! -d "$DIR" ]; then
    echo "'$DIR' directory does not exist. Creating it now..."
    mkdir -p "$DIR"
    if [ $? -ne 0 ]; then
        echo "Error: Failed to create '$DIR' directory."
        exit 1
    fi
fi


cd "$DIR"


if ! cmake -DCMAKE_PREFIX_PATH=$MY_INSTALL_DIR ..; then
    echo "Error: cmake command failed."
    exit 2
fi


if ! make -j 16; then
    echo "Error: make command failed."
    exit 3
fi

echo "Build completed successfully."
