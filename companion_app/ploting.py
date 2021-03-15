import plotly.express as px
import pandas as pd

try:
    with open("collected_ImuData.csv") as f:
        raw_data = pd.read_csv(f)
except FileNotFoundError:
    # VSCode defaults to running in the working directory of the root project so we need a different path
    with open("companion_app/collected_ImuData.csv") as f:
        raw_data = pd.read_csv(f)


# trim data to only the last reset
data = raw_data.loc[raw_data.reset_idx == max(raw_data.reset_idx)]
# accel in 0.1 m gravity -> gravity
data.accel_x = data.accel_x / 10000
data.accel_y = data.accel_y / 10000
data.accel_z = data.accel_z / 10000
# gyro in m°/s per LSB -> deg/s
data.gyro_x = data.gyro_x / 1000
data.gyro_y = data.gyro_y / 1000
data.gyro_z = data.gyro_z / 1000

# plot_fields = ["accel_x", "accel_y", "accel_z"]
plot_fields = ["gyro_x", "gyro_y", "gyro_z"]
# plot_fields = "temperature"
fig = px.scatter(data, x="timestamp", y=plot_fields)
fig.show()
