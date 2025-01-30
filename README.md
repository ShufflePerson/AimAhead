# AimAhead:  Smart Aimbot for Smarter Gaming 🎯

[![GitHub Stars](https://img.shields.io/github/stars/ShufflePerson/AimAhead?style=social)](https://github.com/ShufflePerson/AimAhead)
[![License](https://img.shields.io/badge/License-MIT-brightgreen)](LICENSE) <!-- Add your license file if applicable -->

**Level up your game with AimAhead, the aimbot that uses smart AI to make you a targeting machine.**

AimAhead is all about giving you super-accurate aim, powered by some clever AI.  It watches your screen, spots the bad guys with its AI brain, and then helps you aim right at 'em. Think of it as having a pro gamer built right into your computer, giving you that edge in your favorite games.

## ✨ Cool Stuff You Get

*   **AI Brains for Sharp Aim:**  Forget clunky aim assists – AimAhead uses AI to see enemies and lock onto them fast. It’s like having aim superpowers.
*   **Supercharged for Your NVIDIA Card:**  AimAhead works its magic best with NVIDIA GPUs. When you load a model, it makes it run *crazy fast* using something called TensorRT. This setup happens just once per model, so after that, it's lightning quick every time.
*   **Tweak it Your Way:**  You're in control!  AimAhead has a bunch of settings so you can make it feel just right for *your* style:

    *   **Aimbot On/Off Switch:**  Flip the aimbot on when you need it, off when you don't. Easy.
    *   **Always-On Aim:**  Want aim help all the time?  Just turn this on.
    *   **Pick Your Keys:**  Use whatever keys you like to activate the aimbot and the auto-fire (triggerbot).
    *   **Aim Speed Control:**  Make the aiming faster or slower – dial it in until it feels perfect.

*   **Smart Trigger Finger (Triggerbot):** Aiming is only half the battle, right?  AimAhead’s triggerbot helps with the shooting too:

    *   **Auto-Fire:** When your crosshair is dead-on an enemy, boom, it shoots!
    *   **Recoil Tamer:**  Guns kick back, we know. This triggerbot can actually adjust for recoil a bit, helping you stay on target even when you're laying down fire.
    *   **Recoil Control Settings:** You can even mess with *how much* and *when* it controls the recoil.  Get it just right for your favorite weapons.
    *   **Firing Delay:** Want it to feel a bit more natural?  You can add a tiny delay before it fires automatically.

*   **Sees the Future (Kind Of):**  Okay, not really the future, but AimAhead *predicts* where enemies will be!

    *   **Next-Frame Guessing:** It looks at how enemies were moving, and then guesses where they’ll be next.  This helps you nail shots on targets that are running around.
    *   **Prediction Power Settings:** You can tell it how much data to use to make these guesses.  More data can be more accurate, but less can be faster. Play around and find what feels best!

*   **Targeting Smarts:**  How do you pick which enemy to aim at when there are a bunch?  You decide!

    *   **Target Choices:**
        *   **"Confidence King":** Goes for the enemy the AI is *most* sure about being a bad guy.  Smart choice.
        *   **"Closest Dude":**  Aims for whoever is nearest to your crosshair. Great for quick fights.
    *   **Where to Aim:**
        *   **Heads (Top):**  Go for those satisfying headshots.
        *   **Body (Middle):**  Easier to hit, more consistent damage.

*   **Smooth Aiming, No Jitters:** Ever had your aim jump around like crazy when there are multiple enemies? AimAhead has tech to stop that:

    *   **Jitter Fighter:**  Keeps your aim steady, even when there are lots of targets nearby. No more jumpy aim!
    *   **Jitter Sensitivity Control:** You can tweak how sensitive it is to "jitter."
    *   **Wait-a-Sec Target Swapping:**  If an enemy ducks behind cover for a split second, it won’t immediately jump to a new target.  Keeps things focused.

*   **For Techy Folks (and Helpers!):**  Some extras if you like to tinker or help out with development:

    *   **Auto Data Collection:**  Want to help make AimAhead even smarter? It can automatically save gameplay data to help train the AI. (You can turn this off too).
    *   **Cursor-Smart Performance:** It only runs the AI when you’re actually in-game (cursor hidden). Saves on computer power when you’re not playing.
    *   **Frame Rate Control:**  If you want to limit how hard AimAhead works your system, you can cap its frame rate.
    *   **Model Selector:**  Easily switch between different AI models to try them out.
    *   **Headshot Fine-Tuning:**  Adjust settings to really dial in those perfect headshots.

*   **See What's Happening:**  Get some visual feedback on screen:

    *   **Enemy Outlines:**  See boxes drawn around enemies it spots.  Just to confirm it sees them.
    *   **Confidence Meter:**  It can show you how "sure" the AI is about each enemy it finds.  Just for kicks.

## ⚙️ What You'll Need

*   **NVIDIA Graphics Card:**  Gotta have that NVIDIA power for best performance.
*   **CUDA 12.4:**  Think of this as special software your NVIDIA card needs. Grab it from [NVIDIA CUDA Toolkit Archive](developer.nvidia.com/cuda-12-4-0-download-archive).

## 📂 Getting Models In There

1.  **Drop your ONNX models** into the folder called `models`.
2.  When you fire up AimAhead, it'll see the models you put in there.
3.  **First time loading a model, it does some magic.**  It makes the model super-fast for your GPU. This might take a few minutes the very first time, but it's a one-time thing. After that, models load super quick.

## 🚀 Let's Go!

1.  **Grab the Goods:** Make sure you've got an NVIDIA card and CUDA 12.4 installed ([See "What You'll Need"](#-what-youll-need)).
2.  **Get AimAhead:** Download the latest version from the [GitHub Releases page](https://github.com/ShufflePerson/AimAhead/releases) (if there are releases).
3.  **Models In Place:**  Put your ONNX models into the `models` folder.
4.  **Run It!:**  Start the AimAhead program.
5.  **Set It Up:**  Adjust settings and keys in the program menu until it's just how you want it.
6.  **Game On, Aim Sharp!**

## ⚠️ One Quick Thing... (Important!)

**Play Fair & Smart:**  AimAhead is made for learning and messing around.  Just be cool and remember the rules of the games you play.  Using stuff like this in serious online games *could* get you in trouble with the game makers.  Just a heads-up.

**Still Being Built!**  AimAhead is a work in progress.  Got ideas? Find a bug?  Want to help make it better?  Awesome!  Feel free to jump in and contribute!

---

**Hope you have a blast checking out what AI can do for gaming with AimAhead!**