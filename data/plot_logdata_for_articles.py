import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

sns.set()
sns.set_style('whitegrid')

data_df = pd.read_csv("blackbox.csv", sep=",")

fig = plt.figure(figsize=(10, 8))
titles = ["Roll", "Pitch", "Yaw"]
for i in range(3):
    ax = fig.add_subplot(3, 1, i+1)
    ax.set_ylabel("Angle [rad]")
    ax.set_xlabel("Time [sec]")
    ax.set_title(titles[i])
    ax.plot(data_df["TIME"], data_df["ANGLE_RAD_" + str(i)], marker="",)
    ax.plot(data_df["TIME"], data_df["TARGET_ANGLE_" + str(i)], marker="")
    ax.grid()

plt.subplots_adjust(left=0.1, bottom=0.1, right=0.9, top=0.9, wspace=None, hspace=0.6) 

fig.savefig("rad_angles.png")
plt.close()
