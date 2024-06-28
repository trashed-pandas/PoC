! plot.f90
program plot
    use :: plplot
    implicit none
    integer, parameter :: N     = 2000
    real,    parameter :: XSTEP = 0.01

    real    :: x(N), y(N)
    real    :: xmax, xmin, ymax, ymin
    integer :: i, rc

    xmin = 0.0
    xmax = XSTEP * N
    ymin = -10.0
    ymax = 10.0

    ! Prepare data.
    do i = 1, N
        x(i) = XSTEP * i
        y(i) = 7.5 * sin(x(i))
    end do

    ! Parse and process command line arguments.
    rc = plparseopts(PL_PARSE_FULL)
    if (rc /= 0) stop 'Error: Parsing command-line arguments failed'

    call plscolbg(16, 16, 16) ! Set background colour (RGB).
    call plinit()             ! Initialise PLplot.
    call plcol0(15)           ! Set foreground colour.

    ! Create a labeled box that contains the plot.
    call plenv(xmin, xmax, ymin, ymax, 0, 0)
    call pllab('x', 'y = 7.5 sin(x)', '2D line plot using PLplot' )

    ! Plot the data as line diagram.
    call plline(x, y)

    ! Close PLplot library
    call plend()
end program plot
