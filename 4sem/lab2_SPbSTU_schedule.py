import requests
import datetime
import matplotlib.pyplot as plt


import ApiSpbStuRuz
# import asyncio

# my_date = datetime.datetime.strptime('2011-07-15', '%Y-%m-%d')
# async def main():
#     async with ApiSpbStuRuz.ApiSpbStuRuz() as api:
#         teacher = await api.get_teacher_scheduler_by_id(11310)
#         print(teacher)


def graphic(lessons_massive):
    plt.plot(["Пн", "Вт", "Ср", "Чт", "Пт", "Сб", "Вс"], lessons_massive)
    plt.xlabel('Дни недели')
    plt.ylabel('Количество занятий')
    plt.title('Нагруженность недели')
    plt.show()


def param_input():
    group_num = input("\nEnter a full group number (skip for 5131001/20002): ")
    if group_num == '':
        group_num = "5131001/20002"

    sub_req = requests.get(f'https://ruz.spbstu.ru/api/v1/ruz/search/groups?q={group_num}').json()
    group_id = 0
    for group in sub_req['groups']:
        if group['name'] == group_num:
            group_id = group['id']
    date = input("Enter a date in the format: year-month-day (skip for 2024-3-19): ")
    if date == '':
        date = '2024-3-19'
    return group_id, date


def schedule_output(request):
    print(
        f"\n{datetime.datetime.strptime(request['week']['date_start'], '%Y.%m.%d').strftime('%d.%m.%Y')} - "
        f"{datetime.datetime.strptime(request['week']['date_end'], '%Y.%m.%d').strftime('%d.%m.%Y')}")
    if req['week']['is_odd']:
        print("Нечетная неделя")
    else:
        print("Четная неделя")
    week_days_list = ["Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота", "Воскресенье"]
    day_lessons_number = [0, 0, 0, 0, 0, 0, 0]
    for day in request['days']:
        print(f"\n{week_days_list[int(day['weekday']) - 1]},"
              f" {datetime.datetime.strptime(day['date'], '%Y-%m-%d').strftime('%d.%m.%Y')}\n")
        lesson_number = 0
        for lesson in day['lessons']:
            lesson_number += 1
            print(f"{lesson_number}) {lesson['time_start']}-{lesson['time_end']}", lesson['subject'], '--',
                  lesson['typeObj']['name'])
            if lesson['teachers']:
                for teacher in lesson['teachers']:
                    print(teacher['full_name'])
            if lesson['auditories']:
                for auditory in lesson['auditories']:
                    print(f"{auditory['building']['name']}, {auditory['name']}\n")
        day_lessons_number[int(day['weekday']) - 1] = lesson_number
    graphic(day_lessons_number)


if __name__ == '__main__':
    # search group
    # https://ruz.spbstu.ru/api/v1/ruz/search/groups?q=5131001/20002
    # group
    # https://ruz.spbstu.ru/api/v1/ruz/scheduler/38652?date=2024-3-19
    # search teacher
    # https://ruz.spbstu.ru/api/v1/ruz/search/teachers?q=Писков%20Александр%20Александрович
    # teacher
    # https://ruz.spbstu.ru/api/v1/ruz/teachers/25487/scheduler?date=2024-3-11
    # asyncio.run(main())

    group_id_param, date_param = param_input()

    req = requests.get(f'https://ruz.spbstu.ru/api/v1/ruz/scheduler/{group_id_param}?date={date_param}').json()

    schedule_output(req)
