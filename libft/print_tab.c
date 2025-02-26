/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tab.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:07:23 by throbert          #+#    #+#             */
/*   Updated: 2025/02/26 08:17:41 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"
#include <stdio.h>

void printab(char **tab, const char *fmt)
{
    int i;

    i = 0;
    while (tab && tab[i] && i < 5)
    {
        printf("[%d] : ", i + 1);
        printf(fmt, tab[i]);
        printf("\n");
        i++;
    }
}
