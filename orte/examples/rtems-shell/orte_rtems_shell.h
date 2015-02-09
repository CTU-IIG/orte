#ifndef ORTE_RTEMS_SHELL_H
#define ORTE_RTEMS_SHELL_H

int
ortemanager_main(int argc, char **argv);

int
orte_h_publisher_main(int argc, char **argv);
int
orte_h_subscriber_main(int argc, char **argv);
int
orte_m_subscriber_main(int argc, char **argv);
int
orte_ping_main(int argc, char **argv);
int
orte_spy_main(int argc, char **argv);
int
orte_r_publisher_main(int argc, char **argv);
int
orte_r_subscriber_besteffort_main(int argc, char **argv);
int
orte_r_subscriber_reliable_main(int argc, char **argv);

int
orte_spawn_main(int argc, char **argv);

#endif /*ORTE_RTEMS_SHELL_H*/
