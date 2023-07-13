# radmir-copy-cef-module
Модуль для упрощения работы с CEF интерфейсами.

Функция: SendClientEventF (аналог SendPacket_1), используется чтобы сразу отправлять форматированную строку.
Пример: SendClientEventF(playerid, Interface, CefShow, "interface('Hud').setNewYear(%d)", param);

Функция: SendClientEventWithArgumentsF (аналог SendPacket_2) используется чтобы сразу отправлять форматированную строку.
Пример: SendClientEventWithArgumentsF(playerid, Interface, CefShow, "Auth", "[\"auth\", \"%s\", 1, 1, 0, 0, 1]", param);

Version 2.0

Появилась возможность использования в filterscripts
— Принятие эвентов в filterscripts файле
— Отправка пакета
