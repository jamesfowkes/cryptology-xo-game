#include <TaskAction.h>

static const int s_outputs[] = {3, 4, 5, 6};

static const int s_inputs[] = {A0, A1, A2, A3, A4, A5, 7, 8, 9};

static bool s_grid_state[9];
static int s_match = -1;

static const bool s_patterns[4][9] = {
	{
		true, false, true, 
	 	true, true, false,
	 	false, true, false
	},
	{
		false, false, false,
		false, true, false,
		true, true, true
	},
	{
		false, true, true,
		true, false, false,
		false, true, true
	},
	{
		true, false, false,
		true, false, false,
		false, true, false
	},
};

static void get_grid_state(bool * grid)
{
	int i = 0;

	for (i = 0; i < 9; i++)
	{
		grid[i] = digitalRead(s_inputs[i]) == HIGH;
	}
}

static bool match_patterns(bool const * const p1, bool const * const p2)
{
	bool match = true;

	int i = 0;

	for (i = 0; i < 9; i++)
	{
		match &= (p1[i] == p2[i]);
	}

	return match;
}

static void print_grid(bool const * const grid)
{
	int j = 0;
	int k = 0;

	for (j = 0; j < 3; j++)
	{
		for (k = 0; k < 3; k++)
		{
			Serial.print(grid[(j*3)+k]);
			if (k < 2) { Serial.print(", "); }
		}
		Serial.println("");
	}
}

static void debug_task_fn()
{
	Serial.println("Grid:");

	print_grid(s_grid_state);

	Serial.print("Match: ");
		
	if (s_match >= 0)
	{
		Serial.println(s_match);
	}
	else
	{
		Serial.println("none");
	}
}
static TaskAction s_debug_task(debug_task_fn, 2000, INFINITE_TICKS);

void setup()
{
	int i = 0;
	for (i = 0; i < 9; i++)
	{
		pinMode(s_inputs[i], INPUT);
	}

	for (i = 0; i < 4; i++)
	{
		pinMode(s_outputs[i], OUTPUT);
	}

	Serial.begin(115200);

	for (i = 0; i < 4; i++)
	{
		Serial.print("Grid ");
		Serial.print(i);
		Serial.println(":");

		print_grid(s_patterns[i]);
	}
}

void loop()
{
	get_grid_state(s_grid_state);

	int i = 0;

	s_match = -1;

	for (i = 0; i < 4; i++)
	{
		if (match_patterns(s_grid_state, s_patterns[i]))
		{
			s_match = i;
			digitalWrite(s_outputs[i], HIGH);
		}
		else
		{
			digitalWrite(s_outputs[i], LOW);	
		}
	}

	s_debug_task.tick();
}
