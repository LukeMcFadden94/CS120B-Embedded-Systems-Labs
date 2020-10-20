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

    {'description': 'PINA[1:0]: 14, PINB[1:0]: 14, PINC[1:0]: 14',
    'steps': [ {'inputs': [('PINA',0x14), ('PINB',0x14), ('PINC',0x14)], 'iterations': 2 } ],
    'expected': [('PORTD',0x3C)]
    },
    {'description': 'PINA[1:0]: 2D, PINB[1:0]: 2D, PINC[1:0]: 2D',
    'steps': [ {'inputs': [('PINA',0x2D), ('PINB',0x2D), ('PINC',0x2D)], 'iterations': 2 } ],
    'expected': [('PORTD',0x84)]
    },
    {'description': 'PINA[1:0]: 32, PINB[1:0]: 32, PINC[1:0]: 32',
    'steps': [ {'inputs': [('PINA',0x32), ('PINB',0x32), ('PINC',0x32)], 'iterations': 2 } ],
    'expected': [('PORTD',0x09)]
    },
    {'description': 'PINA[1:0]: 46, PINB[1:0]: 46, PINC[1:0]: 46',
    'steps': [ {'inputs': [('PINA',0x46), ('PINB',0x46), ('PINC',0x46)], 'iterations': 2 } ],
    'expected': [('PORTD',0x45)]
    },
    {'description': 'PINA[1:0]: 0A, PINB[1:0]: 14, PINC[1:0]: 5F',
    'steps': [ {'inputs': [('PINA',0x0A), ('PINB',0x14), ('PINC',0x5F)], 'iterations': 2 } ],
    'expected': [('PORTD',0x7E)]
    },
    {'description': 'PINA[1:0]: 00, PINB[1:0]: 28, PINC[1:0]: 64',
    'steps': [ {'inputs': [('PINA',0x00), ('PINB',0x28), ('PINC',0x64)], 'iterations': 2 } ],
    'expected': [('PORTD',0x8E)]
    },
    {'description': 'PINA[1:0]: 78, PINB[1:0]: 1E, PINC[1:0]: 14',
    'steps': [ {'inputs': [('PINA',0x78), ('PINB',0x1E), ('PINC',0x14)], 'iterations': 2 } ],
    'expected': [('PORTD',0x1F)]
    },0


]

# Optionally you can add a set of "watch" variables these need to be global or static and may need
# to be scoped at the function level (for static variables) if there are naming conflicts. The 
# variables listed here will display everytime you hit (and stop at) a breakpoint
watch = ['main::tempA', 'main::tempB', 'main::tempC', 'main::totalWeight', 'main::weightDiff', 'main::tempD', 'PORTD']
