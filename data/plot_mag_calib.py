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

# fig = plt.figure()
# ax = Axes3D(fig)

# ax.set_xlabel("X")
# ax.set_ylabel("Y")
# ax.set_zlabel("Z")

# ax.plot(before_raw["RAW_MAG_X"], before_raw["RAW_MAG_Y"], before_raw["RAW_MAG_Z"],
#         marker="o", color='g', alpha=0.1, linestyle='None')
# ax.plot(after_raw["RAW_MAG_X"], after_raw["RAW_MAG_Y"], after_raw["RAW_MAG_Z"],
#         marker="o", color='b', alpha=0.1, linestyle='None')
# ax.plot(offsets["CALIB_MAG_X"].tail(), offsets["CALIB_MAG_Y"].tail(), offsets["CALIB_MAG_Z"].tail(),
#         marker="o", color='r', alpha=1, linestyle='None')
# fig.savefig("before_after_raw_mag.png")
# plt.close()

data_1 = np.array([before_raw["RAW_MAG_X"], before_raw["RAW_MAG_Y"], before_raw["RAW_MAG_Z"]])
data_2 = np.array([after_raw["RAW_MAG_X"], after_raw["RAW_MAG_Y"], after_raw["RAW_MAG_Z"]])
data_3 = np.array([offsets["CALIB_MAG_X"].tail(), offsets["CALIB_MAG_Y"].tail(), offsets["CALIB_MAG_Z"].tail()])
data_4 = np.array([0, 0, 0])

def render_frame(angle):
    """data の 3D 散布図を PIL Image に変換して返す"""
    global data_1, data_2, data_3, data_4
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(*data_1, color='b', alpha=0.01)
    ax.scatter(*data_2, color='g', alpha=0.01)
    ax.scatter(*data_3, color='r')
    ax.scatter(*data_4, color='k')
    ax.view_init(30, angle)
    # 軸の設定
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
#     ax.set_xlim(-100, 100)
#     ax.set_ylim(-100, 100)
#     ax.set_zlim(-100, 100)
    # PIL Image に変換
    buf = BytesIO()
    fig.savefig(buf, bbox_inches='tight', pad_inches=0.0)
    plt.close()
    return Image.open(buf)


# images = [render_frame(angle) for angle in range(1)]
# images[0].save('before_after_raw_mag.gif', save_all=True,
#                append_images=images[1:], duration=100, loop=0)
# plt.close()

fig = plt.figure()
plt.plot(np.linspace(0, len(offsets["EPSILON"]), len(
    offsets["EPSILON"])), offsets["EPSILON"])
fig.savefig("epsilon.png")
plt.close()


fig = plt.figure(figsize=(15, 5))

ax_1 = fig.add_subplot(131, aspect="equal")
ax_1.plot(before_raw["RAW_MAG_X"].values, before_raw["RAW_MAG_Z"].values, alpha=0.5, label="Before caliblation")
ax_1.plot(after_raw["RAW_MAG_X"], after_raw["RAW_MAG_Z"], color='g', alpha=0.5, label="After caliblation")
ax_1.scatter(offsets["CALIB_MAG_X"].tail(), offsets["CALIB_MAG_Z"].tail(), color='r', label="Hard-Iron offset")
ax_1.scatter(0, 0, color='k', label="Origin")
ax_1.set_xlabel('X')
ax_1.set_ylabel('Z')
ax_1.set_xlim(-90, 90)
ax_1.set_ylim(-90, 90)
ax_1.legend()
ax_1.set_title("Magnemeter plot X-Z")

ax_2 = fig.add_subplot(132, aspect="equal")
ax_2.plot(before_raw["RAW_MAG_Y"].values, before_raw["RAW_MAG_Z"].values, alpha=0.5, label="Before caliblation")
ax_2.plot(after_raw["RAW_MAG_Y"], after_raw["RAW_MAG_Z"], color='g', alpha=0.5, label="After caliblation")
ax_2.scatter(offsets["CALIB_MAG_Y"].tail(), offsets["CALIB_MAG_Z"].tail(), color='r', alpha=0.5, label="Hard-Iron offset")
ax_2.scatter(0, 0, color='k', label="Origin")
ax_2.set_xlabel('Y')
ax_2.set_ylabel('Z')
ax_2.set_xlim(-90, 90)
ax_2.set_ylim(-90, 90)
ax_2.legend()
ax_2.set_title("Magnemeter plot Y-Z")

ax_3 = fig.add_subplot(133, aspect="equal")
ax_3.plot(before_raw["RAW_MAG_X"].values, before_raw["RAW_MAG_Y"].values, alpha=0.5, label="Before caliblation")
ax_3.plot(after_raw["RAW_MAG_X"], after_raw["RAW_MAG_Y"], color='g', alpha=0.5, label="After caliblation")
ax_3.scatter(offsets["CALIB_MAG_X"].tail(), offsets["CALIB_MAG_Y"].tail(), color='r', alpha=0.1, label="Hard-Iron offset")
ax_3.scatter(0, 0, color='k', label="Origin")
ax_3.set_xlabel('X')
ax_3.set_ylabel('Y')
ax_3.set_xlim(-90, 90)
ax_3.set_ylim(-90, 90)
ax_3.legend()
ax_3.set_title("Magnemeter plot X-Y")

# ax_4 = fig.add_subplot(235, projection='3d')
# ax_4.scatter(*data_1, color='b', alpha=0.1)
# ax_4.scatter(*data_2, color='g', alpha=0.1)
# ax_4.scatter(*data_3, color='r')
# ax_4.scatter(*data_4, color='k')
#ax_4.view_init(30, angle)
# 軸の設定
# ax_4.set_xlabel('X')
# ax_4.set_ylabel('Y')
# ax_4.set_zlabel('Z')

fig.savefig("2d_magcalib.png")
plt.close()