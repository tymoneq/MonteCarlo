import subprocess
import os
import platform
import sys


def run_cpp_app():
    print("🔨 Building C++ application...")

    if not os.path.exists("out/"):
        os.makedirs("out")

    if not os.path.exists("build/"):
        os.makedirs("build")

    os.chdir("build")

    # Configure and build
    subprocess.run(["cmake", ".."], check=True)
    subprocess.run(["cmake", "--build", ".", "--config", "Release"], check=True)

    # Find the executable
    if platform.system() == "Windows":
        exe_path = (
            "Debug/myapp.exe" if os.path.exists("Debug/myapp.exe") else "myapp.exe"
        )
    else:
        exe_path = "./myapp"

    print("✅ Build successful! Running C++ application...")
    subprocess.run([exe_path, "data.in"], check=True)

    os.chdir("../")


def run_python_app():
    print("🐍 Running Python visualization...")

    # Use sys.executable to ensure we use the same Python interpreter
    subprocess.run([sys.executable, "visualize.py"], check=True)


if __name__ == "__main__":
    try:
        run_cpp_app()
        run_python_app()
        print("🎉 All tasks completed successfully!")
    except Exception as e:
        print(f"❌ Error: {e}")
        sys.exit(1)
