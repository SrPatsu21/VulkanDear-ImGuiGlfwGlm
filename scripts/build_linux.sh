set -e

mkdir -p build
cd build
cmake ..
cmake --build .

cat << 'EOF' > run.sh
HERE="$(dirname "$(readlink -f "$0")")"

export GTK_THEME=Adwaita
export XDG_DATA_DIRS="$HERE/lib/linux/share:/usr/share"
export GDK_BACKEND=wayland,x11
export LD_LIBRARY_PATH="$HERE/lib/linux:$LD_LIBRARY_PATH"

"$HERE/ProjectD.out"
EOF
chmod +x run.sh