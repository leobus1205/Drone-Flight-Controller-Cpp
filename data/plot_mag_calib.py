import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from io import BytesIO
from PIL import Image
from mpl_toolkits.mplot3d import Axes3D

after_raw = pd.read_csv("after_raw_mag_data.csv", sep=",")
before_raw = pd.read_csv("before_raw_mag_data.csv", sep=",")
offsets = pd.read_csv("mag_offsets_data.csv", sep=",")

sns.set_style("darkgrid")

fig = plt.figure()
ax = Axes3D(fig)

ax.set_xlabel("X")
ax.set_ylabel("Y")
ax.set_zlabel("Z")

ax.plot(before_raw["RAW_MAG_X"], before_raw["RAW_MAG_Y"], before_raw["RAW_MAG_Z"],
        marker="o", color='g', alpha=0.1, linestyle='None')
ax.plot(after_raw["RAW_MAG_X"], after_raw["RAW_MAG_Y"], after_raw["RAW_MAG_Z"],
        marker="o", color='b', alpha=0.1, linestyle='None')
ax.plot(offsets["CALIB_MAG_X"].tail(), offsets["CALIB_MAG_Y"].tail(), offsets["CALIB_MAG_Z"].tail(),
        marker="o", color='r', alpha=1, linestyle='None')
fig.savefig("before_after_raw_mag.png")

data_1 = np.array([before_raw["RAW_MAG_X"], before_raw["RAW_MAG_Y"], before_raw["RAW_MAG_Z"]])
data_2 = np.array([after_raw["RAW_MAG_X"], before_raw["RAW_MAG_Y"], before_raw["RAW_MAG_Z"]])
data_3 = np.array([offsets["CALIB_MAG_X"].tail(), offsets["CALIB_MAG_Y"].tail(), offsets["CALIB_MAG_Z"].tail()])
data_4 = np.array([0, 0, 0])

def render_frame(angle):
    """data の 3D 散布図を PIL Image に変換して返す"""
    global data_1, data_2, data_3, data_4
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(*data_1, color='b', alpha=0.1)
    ax.scatter(*data_2, color='g', alpha=0.1)
    ax.scatter(*data_3, color='r')
    ax.scatter(*data_4, color='k')
    ax.view_init(30, angle)
    plt.close()
    # 軸の設定
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    #ax.set_xlim(-3, 3)
    #ax.set_ylim(-3, 3)
    #ax.set_zlim(-3, 3)
    # PIL Image に変換
    buf = BytesIO()
    fig.savefig(buf, bbox_inches='tight', pad_inches=0.0)
    return Image.open(buf)


images = [render_frame(angle) for angle in range(360)]
images[0].save('before_after_raw_mag.gif', save_all=True,
               append_images=images[1:], duration=100, loop=0)
plt.close()

fig = plt.figure()
plt.plot(np.linspace(0, len(offsets["EPSILON"]), len(
    offsets["EPSILON"])), offsets["EPSILON"])
fig.savefig("epsilon.png")
plt.close()
