/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
/**
    \brief Generic helpers for string checks and simple queries.
*/
class LogZ_Utils
{
	/**
	    \brief Check that string consists only of digits 0-9.
	    \details Returns false for empty string.
	*/
	static bool IsDigitsOnly(string s)
	{
		if (s == string.Empty)
			return false;

		int len = s.Length();
		for (int i = 0; i < len; i++) {
			int c = s.Get(i).ToAscii();
			if (c < 48 || c > 57) // '0'..'9'
				return false;
		}

		return true;
	}

	/**
	    \brief Check if string contains any ASCII letter A-Z or a-z.
	*/
	static bool HasAlpha(string s)
	{
		if (s == string.Empty)
			return false;

		int len = s.Length();
		for (int i = 0; i < len; i++) {
			int c = s.Get(i).ToAscii();
			if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) // 'A'..'Z' or 'a'..'z'
				return true;
		}

		return false;
	}

	/**
	    \brief Distance between two objects as string.
	    \details Returns "0" if any object is null.
	*/
	static string Distance(Object a, Object b)
	{
		if (!a || !b)
			return "0";

		return vector.Distance(a.GetPosition(), b.GetPosition()).ToString();
	}

	/**
	    \brief Find player by exact identity name.
	    \details Returns first matching Man or null.
	*/
	static Man GetManByName(string name)
	{
		if (name == string.Empty)
			return null;

		array<Man> mans = new array<Man>();
		g_Game.GetPlayers(mans);

		foreach (Man m : mans) {
			if (!m)
				continue;

			PlayerIdentity id = m.GetIdentity();
			if (id && id.GetName() == name)
				return m;
		}

		return null;
	}
}
