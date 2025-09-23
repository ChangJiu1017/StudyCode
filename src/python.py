import calendar
from datetime import datetime

# 获取当前日期
now = datetime.now()

# 获取当前年份和月份
year = now.year
month = now.month

# 创建一个TextCalendar实例
cal = calendar.TextCalendar()

# 打印当前月份的日历
cal.prmonth(year, month)