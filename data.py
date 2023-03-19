import random

f = lambda: random.randint(0, 100)
# Do smthn with this wilson
data = {
  "Net Energy": 69,
  "Energy Used": 12,
  "Energy Gained": 81,
  "Water Used": 69,
  "Temperature": 7,
}

solar_panel = lambda: {
  "labels": ["00:00", "04:00", "8:00", "12:00", "16:00", "20:00", "00:00"],
  "data": [f(), f(), f(), f(), f(), f(), f()],
}
