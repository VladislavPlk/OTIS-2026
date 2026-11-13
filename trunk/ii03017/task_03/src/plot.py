import csv
from pathlib import Path
import matplotlib.pyplot as plt

csv_path = Path("pid_results.csv")
if not csv_path.exists():
    raise SystemExit("Не найден pid_results.csv. Сначала запустите программу.")

with csv_path.open("r", newline="", encoding="utf-8") as stream:
    rows = list(csv.DictReader(stream))

if not rows:
    raise SystemExit("CSV-файл не содержит результатов.")

time = [float(row["time"]) for row in rows]
setpoint = [float(row["setpoint"]) for row in rows]
temperature = [float(row["temperature"]) for row in rows]
control = [float(row["control"]) for row in rows]

fig, (temperature_ax, control_ax) = plt.subplots(
    2, 1, figsize=(10, 7), sharex=True
)

temperature_ax.plot(time, setpoint, linestyle="--", label="Заданная температура")
temperature_ax.plot(time, temperature, label="Температура объекта")
temperature_ax.set_title("Переходный процесс системы с ПИД-регулятором")
temperature_ax.set_ylabel("Температура")
temperature_ax.grid(True)
temperature_ax.legend()

control_ax.plot(time, control, label="Управляющее воздействие")
control_ax.set_xlabel("Время")
control_ax.set_ylabel("u(t)")
control_ax.grid(True)
control_ax.legend()

fig.tight_layout()
fig.savefig("pid_plot.png", dpi=160)
print("График сохранён в pid_plot.png")
plt.show()
