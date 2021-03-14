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

# plot_fields = ["accel_x", "accel_y", "accel_z"]
plot_fields = ["gyro_x", "gyro_y", "gyro_z"]
# plot_fields = "temperature"
fig = px.scatter(data, x="timestamp", y=plot_fields)
fig.show()
