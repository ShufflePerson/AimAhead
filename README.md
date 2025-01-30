# AimAhead: AI-Powered Precision for Gaming 🎯

[![GitHub Stars](https://img.shields.io/github/stars/ShufflePerson/AimAhead?style=social)](https://github.com/ShufflePerson/AimAhead)
[![License](https://img.shields.io/badge/License-MIT-brightgreen)](LICENSE) <!-- Add your license file if applicable -->

**Enhance your gameplay with AimAhead, the intelligent AI aimbot that brings unparalleled accuracy to your fingertips.**

AimAhead utilizes the power of cutting-edge AI to identify and target enemies directly from your screen. By processing real-time screen captures through a sophisticated AI model, AimAhead provides precise and responsive aiming assistance, giving you the edge you need in competitive gaming.

## ✨ Key Features

*   **AI-Powered Precision:**  Experience the next level of aiming accuracy driven by advanced artificial intelligence. AimAhead intelligently analyzes your screen to detect and lock onto targets with remarkable speed and precision.
*   **Model Optimization with TensorRT:**  AimAhead automatically compiles your chosen AI model into a TensorRT engine, leveraging the full potential of your NVIDIA GPU for optimized performance and lightning-fast target acquisition. This optimization process is a one-time setup, ensuring smooth and efficient operation thereafter.
*   **Highly Customizable Aimbot:** Tailor AimAhead to your exact playstyle with a comprehensive suite of adjustable settings:

    *   **Toggleable Aimbot:** Activate or deactivate the aimbot functionality instantly.
    *   **Always Aim Mode:**  Engage continuous aim assistance for constant target tracking.
    *   **Customizable Keybindings:**  Set your preferred keys for activating the aimbot and triggerbot features.
    *   **Adjustable Sensitivity:** Fine-tune the aiming speed to match your reflexes and preferences.

*   **Intelligent Triggerbot System:**  Go beyond aiming with the integrated triggerbot, designed for smart and responsive firing:

    *   **Auto Trigger:** Automatically fire when your crosshair aligns perfectly with an enemy.
    *   **Recoil Compensation:**  Intelligently adjust the auto trigger to compensate for weapon recoil, maintaining accuracy during sustained fire.
    *   **Recoil Adjustment Control:**  Customize the amount and timing of recoil compensation for different weapons and scenarios.
    *   **Trigger Delay:**  Introduce a slight delay to the auto trigger for a more human-like firing pattern.

*   **Predictive Aiming:** Stay one step ahead of your opponents with predictive targeting:

    *   **Next Frame Prediction:**  Anticipate enemy movement by analyzing previous frames to predict their position in the next frame, leading to more accurate shots on moving targets.
    *   **Prediction Frame Control:**  Adjust the number of frames used for prediction to balance responsiveness and accuracy based on your game and system.

*   **Smart Target Selection:**  Choose the targeting behavior that suits your strategy:

    *   **Targeting Modes:**
        *   **OBJECTIVE:** Prioritizes targets with the highest AI confidence, ensuring you focus on the most likely threats.
        *   **CLOSEST:**  Targets the enemy closest to your crosshair for immediate engagement in fast-paced situations.
    *   **Aim Positions:**
        *   **TOP (Headshots):**  Maximize damage by aiming directly for the head.
        *   **MIDDLE (Torso):**  A more forgiving aim position for consistent hits.

*   **Anti-Jitter Technology:**  Maintain smooth and stable aiming, even in chaotic engagements:

    *   **Jitter Reduction:**  Prevents erratic target switching when multiple enemies are nearby, ensuring a focused and consistent aim.
    *   **Jitter Sensitivity:**  Define the threshold for jitter detection to customize the smoothness of target switching.
    *   **Target Reappearance Wait Time:**  Set a delay before switching to a new target that briefly disappears and reappears, preventing unwanted target shifts.

*   **Developer & Training Features:** (For advanced users and potential contributors)

    *   **Automatic Training Data Collection:**  Effortlessly contribute to improving AimAhead's AI by automatically saving gameplay data for model training.
    *   **Hidden Cursor Detection:**  Optimize performance by only running the AI model when the cursor is hidden during active gameplay.
    *   **FPS Cap:**  Control resource usage by setting a frame rate cap for AimAhead's processing.
    *   **Model Selection:** Easily switch between different AI models to experiment and find the best performance.
    *   **Minimum Top Margin:**  Fine-tune headshot accuracy by adjusting the minimum margin for the "TOP" aim position.

*   **Visual Feedback:**  Stay informed with on-screen visual aids:

    *   **Target Drawing:**  Visually highlight detected enemies on your screen for clear target confirmation.
    *   **Confidence Display:**  See the AI's confidence level for each target, providing insight into target accuracy.

## ⚙️ Requirements

*   **NVIDIA GPU:**  For optimal performance and TensorRT acceleration.
*   **CUDA 12.4:**  Ensure you have the correct CUDA Toolkit version installed. Download it from the [NVIDIA CUDA Toolkit Archive](developer.nvidia.com/cuda-12-4-0-download-archive).

## 📂 Model Management

1.  **Place your ONNX models** into the `models` directory.
2.  Upon launching AimAhead, it will automatically detect and list the models in the configuration menu.
3.  **First-time model loading will trigger TensorRT compilation.** This optimization process may take 5-10 minutes but is performed only once per model, creating an optimized engine file for your GPU. Subsequent loading will be significantly faster.

## 🚀 Getting Started

1.  **Install the Requirements:** Ensure you have an NVIDIA GPU and CUDA 12.4 installed as outlined in the [Requirements](#-requirements) section.
2.  **Download AimAhead:** Obtain the latest release from the [GitHub Releases page](https://github.com/ShufflePerson/AimAhead/releases) (if you have releases set up).
3.  **Place Models:** Put your ONNX models in the `models` directory.
4.  **Run AimAhead:** Launch the AimAhead executable.
5.  **Configure:**  Use the in-game or configuration menu to adjust settings and keybindings to your liking.
6.  **Enjoy Enhanced Gameplay!**

## ⚠️ Important Disclaimer

**Use Responsibly:** AimAhead is intended for educational and experimental purposes. Please be aware of and respect the terms of service and rules of any game you play.  Using aimbots in competitive environments may violate game rules and could result in penalties.

**Development and Contributions:** AimAhead is under active development. Contributions, bug reports, and feature requests are welcome! Feel free to fork the repository and submit pull requests.

---

**We hope you find AimAhead to be a valuable tool for exploring the capabilities of AI in gaming!**