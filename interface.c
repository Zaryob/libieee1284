/*
 * libieee1284 - IEEE 1284 library
 * Copyright (C) 2001  Tim Waugh <twaugh@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "detect.h"
#include "ieee1284.h"

int ieee1284_read_data (struct parport *port)
{
  struct parport_internal *priv = port->priv;
  return priv->fn->read_data (priv);
}

void ieee1284_write_data (struct parport *port, unsigned char st)
{
  struct parport_internal *priv = port->priv;
  priv->fn->write_data (priv, st);
}

void ieee1284_data_dir (struct parport *port, int reverse)
{
  struct parport_internal *priv = port->priv;
  priv->fn->data_dir (priv, reverse);
}

int ieee1284_read_status (struct parport *port)
{
  struct parport_internal *priv = port->priv;
  return priv->fn->read_status (priv);
}

int ieee1284_wait_status (struct parport *port,
			  unsigned char mask,
			  unsigned char val,
			  struct timeval *timeout)
{
  struct parport_internal *priv = port->priv;
  return priv->fn->wait_status (priv, mask, val, timeout);
}

int ieee1284_read_control (struct parport *port)
{
  struct parport_internal *priv = port->priv;
  return priv->fn->read_control (priv);
}

void ieee1284_write_control (struct parport *port, unsigned char ct)
{
  struct parport_internal *priv = port->priv;
  priv->fn->write_control (priv, ct);
}

void ieee1284_frob_control (struct parport *port, unsigned char mask,
			    unsigned char val)
{
  struct parport_internal *priv = port->priv;
  return priv->fn->frob_control (priv, mask, val);
}

int ieee1284_do_nack_handshake (struct parport *port,
				unsigned char ct_before,
				unsigned char ct_after,
				struct timeval *timeout)
{
  struct parport_internal *priv = port->priv;
  return priv->fn->do_nack_handshake (priv, ct_before, ct_after, timeout);
}

int ieee1284_negotiate (struct parport *port, int mode)
{
  struct parport_internal *priv = port->priv;
  return priv->fn->negotiate (priv, mode);
}

void ieee1284_terminate (struct parport *port)
{
  struct parport_internal *priv = port->priv;
  priv->fn->terminate (priv);
}

int ieee1284_ecp_fwd_to_rev (struct parport *port)
{
  struct parport_internal *priv = port->priv;
  return priv->fn->ecp_fwd_to_rev (priv);
}

int ieee1284_ecp_rev_to_fwd (struct parport *port)
{
  struct parport_internal *priv = port->priv;
  return priv->fn->ecp_rev_to_fwd (priv);
}

ssize_t ieee1284_nibble_read (struct parport *port, char *buffer,
			      size_t len)
{
  struct parport_internal *priv = port->priv;
  return priv->fn->nibble_read (priv, buffer, len);
}

ssize_t ieee1284_compat_write (struct parport *port,
			       const char *buffer, size_t len)
{
  struct parport_internal *priv = port->priv;
  return priv->fn->compat_write (priv, buffer, len);
}

ssize_t ieee1284_byte_read (struct parport *port, char *buffer,
			    size_t len)
{
  struct parport_internal *priv = port->priv;
  return priv->fn->byte_read (priv, buffer, len);
}

ssize_t ieee1284_epp_read_data (struct parport *port, char *buffer,
				size_t len)
{
  struct parport_internal *priv = port->priv;
  return priv->fn->epp_read_data (priv, buffer, len);
}

ssize_t ieee1284_epp_write_data (struct parport *port,
				 const char *buffer, size_t len)
{
  struct parport_internal *priv = port->priv;
  return priv->fn->epp_write_data (priv, buffer, len);
}

ssize_t ieee1284_epp_read_addr (struct parport *port, char *buffer,
				size_t len)
{
  struct parport_internal *priv = port->priv;
  return priv->fn->epp_read_addr (priv, buffer, len);
}

ssize_t ieee1284_epp_write_addr (struct parport *port,
				 const char *buffer, size_t len)
{
  struct parport_internal *priv = port->priv;
  return priv->fn->epp_write_addr (priv, buffer, len);
}

ssize_t ieee1284_ecp_read_data (struct parport *port, char *buffer,
				size_t len)
{
  struct parport_internal *priv = port->priv;
  return priv->fn->ecp_read_data (priv, buffer, len);
}

ssize_t ieee1284_ecp_write_data (struct parport *port,
				 const char *buffer, size_t len)
{
  struct parport_internal *priv = port->priv;
  return priv->fn->ecp_write_data (priv, buffer, len);
}

ssize_t ieee1284_ecp_write_addr (struct parport *port,
				 const char *buffer, size_t len)
{
  struct parport_internal *priv = port->priv;
  return priv->fn->ecp_write_addr (priv, buffer, len);
}
