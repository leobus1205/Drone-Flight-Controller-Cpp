import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

data_df = pd.read_csv("test_run_descrete_pid.csv", sep=",")

fig = plt.figure()
ax = fig.add_subplot(111)
ax.plot(np.linspace(0, len(data_df["TARGET_X"]), len(
    data_df["TARGET_X"])), data_df["TARGET_X"])
plt.plot(data_df["OUT_X"])
fig.savefig("test_pid.png")
plt.close()