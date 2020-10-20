# Array of tests to run (in order)
# Each test contains
#   description - 
#   steps - A list of steps to perform, each step can have
#       inputs - A list of tuples for the inputs to apply at that step
#       *time - The time (in ms) to wait before continuing to the next step 
#           and before checking expected values for this step. The time should be a multiple of
#           the period of the system
#       *iterations - The number of clock ticks to wait (periods)
#       expected - The expected value at the end of this step (after the "time" has elapsed.) 
#           If this value is incorrect the test will fail early before completing.
#       * only one of these should be used
#   expected - The expected output (as a list of tuples) at the end of this test
# An example set of tests is shown below. It is important to note that these tests are not "unit tests" in 
# that they are not ran in isolation but in the order shown and the state of the device is not reset or 
# altered in between executions (unless preconditions are used).
tests = [ 

    # Test weight values

    {'description': 'PINA[1:0]: 28, PINB[1:0]: 28, PINC[1:0]: 28',
    'steps': [ {'inputs': [('PINA',0x28), ('PINB',0x28), ('PINC',0x28)], 'iterations': 2 } ],
    'expected': [('PORTD',0x78)]
    },
    {'description': 'PINA[1:0]: 00, PINB[1:0]: 32, PINC[1:0]: 00',
    'steps': [ {'inputs': [('PINA',0x00), ('PINB',0x32), ('PINC',0x00)], 'iterations': 2 } ],
    'expected': [('PORTD',0x30)]
    },
    {'description': 'PINA[1:0]: 2D, PINB[1:0]: 2D, PINC[1:0]: 2D',
    'steps': [ {'inputs': [('PINA',0x2D), ('PINB',0x2D), ('PINC',0x2D)], 'iterations': 2 } ],
    'expected': [('PORTD',0x84)]
    },
    {'description': 'PINA[1:0]: 32, PINB[1:0]: 32, PINC[1:0]: 32',
    'steps': [ {'inputs': [('PINA',0x32), ('PINB',0x32), ('PINC',0x32)], 'iterations': 2 } ],
    'expected': [('PORTD',0x95)]
    },
    {'description': 'PINA[1:0]: 3C, PINB[1:0]: 3C, PINC[1:0]: 3C',
    'steps': [ {'inputs': [('PINA',0x3C), ('PINB',0x3C), ('PINC',0x3C)], 'iterations': 2 } ],
    'expected': [('PORTD',0xB5)]
    },
    {'description': 'PINA[1:0]: 14, PINB[1:0]: 0A, PINC[1:0]: 64',
    'steps': [ {'inputs': [('PINA',0x14), ('PINB',0x0A), ('PINC',0x64)], 'iterations': 2 } ],
    'expected': [('PORTD',0x80)]
    },
    {'description': 'PINA[1:0]: 6E, PINB[1:0]: 1E, PINC[1:0]: 00',
    'steps': [ {'inputs': [('PINA',0x6E), ('PINB',0x1E), ('PINC',0x00)], 'iterations': 2 } ],
    'expected': [('PORTD',0x8E)]
    },
    {'description': 'PINA[1:0]: 19, PINB[1:0]: 2D, PINC[1:0]: 6E',
    'steps': [ {'inputs': [('PINA',0x19), ('PINB',0x2D), ('PINC',0x6E)], 'iterations': 2 } ],
    'expected': [('PORTD',0xB7)]
    },


]

# Optionally you can add a set of "watch" variables these need to be global or static and may need
# to be scoped at the function level (for static variables) if there are naming conflicts. The 
# variables listed here will display everytime you hit (and stop at) a breakpoint
watch = ['main::tempA, tempB, tempC','PORTD']
